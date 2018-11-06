#include "McAudioPlayerWidget.h"
#include "ui_McAudioPlayer.h"
#include <qmediaplayer.h>
#include <qscrollbar.h>
#include <qurl.h>
#include <qfile.h>
#include <QMessageBox>
#include <qmenu.h>
#include <qsettings.h>
#include <QTime>
#include "McGlobal.h"
#include "McSonglistModel.h"
#include "McSonglistDelegate.h"
#include "McSongModel.h"
#include "McSongDelegate.h"
#include "McPlaylistModel.h"
#include "McPlaylistDelegate.h"
#include "UntranslatedLyricWidget.h"
#include "TranslatedLyricWidget.h"

struct McAudioPlayerWidgetData : public Ui::McAudioPlayer {
	QMediaPlayer *m_mediaPlayer;
	McSonglistModel *m_songlistModel;
	McSongModel *m_songModel;
	McPlaylistModel *m_playlistModel;
	QScopedPointer<LyricWidget> m_lyricWidget;
	QMenu *m_addToMenu;
	bool m_isPaused;
	int m_playMode;
};

McAudioPlayerWidget::McAudioPlayerWidget(QObject *parent)
	: QObject(parent)
	, d(new McAudioPlayerWidgetData)
{
	d->m_isPaused = true;
}

McAudioPlayerWidget::~McAudioPlayerWidget(){
	d->m_playlistModel->quit();
	QSettings setting(qApp->applicationDirPath() + "/data/init.ini", QSettings::IniFormat);
	setting.beginGroup("MyAudioPlayer");
	setting.setValue("currentSonglistIndex", d->songlistListView->currentIndex().row());
	setting.setValue("currentMusicIndex", d->playlistListView->currentIndex().row());
	setting.setValue("playMode", d->m_playMode);
	//setting.setValue("isTranslatedLyric", d->m_isTranslatedLyric);
	//setting.setValue("isShowLyric", d->m_isShowLyric);
	setting.endGroup();
}

bool McAudioPlayerWidget::init(QWidget *w) {
	QSettings setting(qApp->applicationDirPath() + "/data/init.ini", QSettings::IniFormat);
	setting.beginGroup("MyAudioPlayer");
	int currentSonglistIndex = setting.value("currentSonglistIndex", 0).toInt();
	int currentMusicIndex = setting.value("currentMusicIndex", -1).toInt();
	int playMode = setting.value("playMode", PlayMode::Shuffle).toInt();
	//bool isTranslatedLyric = setting.value("isTranslatedLyric", false).toBool();
	//bool isShowLyric = setting.value("isShowLyric", false).toBool();
	setting.endGroup();

	d->setupUi(w);

	// 默认状态只显示查找按钮
	d->networkSearchBackButton->hide();
	d->searchSrcBox->hide();
	QLineEdit *lineEdit = new QLineEdit(w);	// 用于搜索网络歌曲的输入框，默认不显示
	lineEdit->setFixedSize(151, 20);
	lineEdit->hide();
	// 默认状态下不显示上一页和下一页
	d->searchBackButton->hide();
	d->searchNextButton->hide();

	loadStyle(d->songTableView->verticalScrollBar(), ":/QSS/data/qss/Scrollbar.qss");
	loadStyle(d->songlistListView->verticalScrollBar(), ":/QSS/data/qss/Scrollbar.qss");
	loadStyle(d->playlistListView->verticalScrollBar(), ":/QSS/data/qss/Scrollbar.qss");
	loadStyle(d->playButton, ":/QSS/data/qss/PlayButton.qss");

	// 初始化
	d->m_mediaPlayer = new QMediaPlayer(this);
	d->m_playMode = playMode;
	updatePlayOrder();
	// 初始化歌单
	d->m_songlistModel = new McSonglistModel(this);
	if (!d->m_songlistModel->init())
		return false;
	d->songlistListView->setModel(d->m_songlistModel);
	McSonglistDelegate *songlistDelegate = new McSonglistDelegate(this);
	d->songlistListView->setItemDelegate(songlistDelegate);
	d->m_songlistModel->loadSonglist();
	QModelIndex index = d->m_songlistModel->index(currentSonglistIndex);
	d->songlistListView->setCurrentIndex(index);
	QString songlist = d->m_songlistModel->songlistName(index);

	QMenu *songlistMenu = new QMenu(w);
	QAction *renameAction = songlistMenu->addAction("重命名");
	songlistMenu->addSeparator();
	QAction *delAction = songlistMenu->addAction("删除");

	connect(d->songlistListView, &QListView::customContextMenuRequested, [this, songlistMenu](const QPoint &pos) {
		showMenu(d->songlistListView, pos, songlistMenu);
	});
	connect(renameAction, &QAction::triggered, [this]() {
		QModelIndex index = d->songlistListView->selectionModel()->currentIndex();
		d->songlistListView->edit(index);
	});
	connect(delAction, &QAction::triggered, [this]() {
		QModelIndex index = d->songlistListView->selectionModel()->currentIndex();
		if (!d->m_songlistModel->isDeleteSonglist(index))
			return;
		QString songlist = d->m_songlistModel->songlistName(index);
		if (songlist == d->currentListLabel->text())
			d->m_songModel->clear();
		d->m_songlistModel->deleteSonglist(index);
	});
	// 初始化歌曲列表
	d->m_songModel = new McSongModel(this);
	if (!d->m_songModel->init())
		return false;
	d->songTableView->setModel(d->m_songModel);
	McSongDelegate *songDelegate = new McSongDelegate(this);
	d->songTableView->setItemDelegate(songDelegate);
	loadSong(songlist);

	d->currentListLabel->setText(songlist);

	// 初始化播放列表
	d->m_playlistModel = new McPlaylistModel(this);
	if (!d->m_playlistModel->init())
		return false;
	d->playlistListView->setModel(d->m_playlistModel);
	McPlaylistDelegate *playlistDelegate = new McPlaylistDelegate(this);
	d->playlistListView->setItemDelegate(playlistDelegate);
	d->playlistListView->setCurrentIndex(d->m_playlistModel->index(currentMusicIndex));
	d->playlistListView->hide();
	setPlaylistCount(d->m_playlistModel->rowCount());
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

	QMenu *songMenu = new QMenu(w);
	d->m_addToMenu = songMenu->addMenu("添加到");
	for (int i = 0; i < d->m_songlistModel->rowCount(); ++i) {
		QModelIndex index = d->m_songlistModel->index(i);
		QString songlist = d->m_songlistModel->songlistName(index);
		addAddToMenu(songlist);
	}

	// QMediaPlayer
	connect(d->m_mediaPlayer, &QMediaPlayer::audioAvailableChanged, [this](bool b) {
		if (b) {
			play();
		}
	});
	connect(d->m_mediaPlayer, &QMediaPlayer::stateChanged, [this](QMediaPlayer::State state) {
		if (state == QMediaPlayer::PlayingState) {
			d->playButton->setToolTip(tr("Pause"));
			d->playButton->setIcon(QIcon(":/McAudioPlayer/data/icon/pause.png"));
			d->playButton->setAccessibleName("pause");
		}
		else {
			d->playButton->setToolTip(tr("Play"));
			d->playButton->setIcon(QIcon(":/McAudioPlayer/data/icon/play.png"));
			d->playButton->setAccessibleName("play");
		}
		loadStyle(d->playButton, ":/QSS/data/qss/PlayButton.qss");
	});
	connect(d->playButton, &QPushButton::clicked, this, &McAudioPlayerWidget::togglePlayback);
	connect(d->previousButton, &QPushButton::clicked, this, &McAudioPlayerWidget::playPrevious);
	connect(d->nextButton, &QPushButton::clicked, [this]() {
		playNext();
	});
	connect(d->playOrderButton, &QPushButton::clicked, [this]() {
		switch (d->m_playMode)
		{
		case McAudioPlayerWidget::Shuffle:
			d->m_playMode = McAudioPlayerWidget::Repeat;
			break;
		case McAudioPlayerWidget::Repeat:
			d->m_playMode = McAudioPlayerWidget::Single;
			break;
		case McAudioPlayerWidget::Single:
			d->m_playMode = McAudioPlayerWidget::Shuffle;
			break;
		default:
			break;
		}
		updatePlayOrder();
		qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
	});
	// 歌单列表
	connect(d->addSonglistButton, &QPushButton::clicked, [this]() {
		QModelIndex index = d->m_songlistModel->addSonglist();
		d->songlistListView->edit(index);
	});
	connect(d->songlistListView, &QListView::clicked, [this](const QModelIndex &index) {
		// 这个信号只会在当前项发生变化之后发出，如果仅仅是当前项的文本被改变，不会发出该信号
		QString songlist = d->m_songlistModel->songlistName(index);
		d->currentListLabel->setText(songlist);
		// 该信号一旦发出，标志当前项一定发生了改变，重新加载歌单
		loadSong(songlist);
	});
	// 网络搜索
	connect(d->networkSearchButton, &QPushButton::clicked, [this, lineEdit]() {
		d->horizontalLayout->replaceWidget(d->networkSearchButton, lineEdit);
		lineEdit->show();
		d->networkSearchBackButton->show();
		d->searchSrcBox->show();
		d->networkSearchButton->hide();
	});
	connect(d->networkSearchBackButton, &QPushButton::clicked, [this, lineEdit]() {
		d->horizontalLayout->replaceWidget(lineEdit, d->networkSearchButton);
		lineEdit->hide();
		d->networkSearchBackButton->hide();
		d->searchSrcBox->hide();
		d->networkSearchButton->show();
	});
	// 歌曲列表
	connect(d->m_addToMenu, &QMenu::triggered, [this](QAction *action) {
		QString songlist = action->text();
		int index = d->m_songlistModel->songlistIndex(songlist);
		if (index < 0)
			return;
		QModelIndexList indexList = d->songTableView->selectionModel()->selectedRows();
		if (indexList.isEmpty())
			return;
		d->songTableView->model()->addTo(index, indexList);
	});
	connect(songDelegate, &McSongDelegate::addTo, [this](const QPoint &pos, const QModelIndex &index) {
		QItemSelectionModel *itemSelectionModel = d->songTableView->selectionModel();
		if(itemSelectionModel->hasSelection())
			itemSelectionModel->setCurrentIndex(index, QItemSelectionModel::Current);
		else
			d->songTableView->setCurrentIndex(index);
		disabledAddToMenu(d->m_songlistModel->songlistName(d->songlistListView->currentIndex()));
		showMenu(d->songTableView, pos, d->m_addToMenu);
	});
	connect(songDelegate, &McSongDelegate::play, [this](const QPoint &pos, const QModelIndex &index) {
		if (d->m_playlistModel->rowCount() != d->m_songModel->rowCount() || !d->m_playlistModel->isExists(d->m_songModel->id(index))) {
			int count = d->m_playlistModel->addSong(d->m_songModel->datas());
			setPlaylistCount(count);
		}
		play(index);
	});
	// 播放列表
	connect(d->playlistButton, &QPushButton::clicked, [this]() {
		d->playlistListView->setVisible(!d->playlistListView->isVisible());
	});
	connect(playlistDelegate, &McPlaylistDelegate::play, [this](const QPoint &pos, const QModelIndex &index) {
		play(index);
	});

	return true;
}

void McAudioPlayerWidget::loadStyle(QWidget *widget, const QString &fileName) {
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) {
		printf("qss file name error!\n");
		fflush(stdout);
		return;
	}
	widget->setStyleSheet(file.readAll());
	file.close();
}

void McAudioPlayerWidget::play(const QString &link) {
	QUrl url;
	if (link.startsWith("http"))
		url = QUrl(link);
	else {
		url = QUrl::fromLocalFile(link);
	}

	d->m_mediaPlayer->setMedia(url);
}

// 暂停整个播放操作
void McAudioPlayerWidget::pause() {
	d->m_mediaPlayer->pause();		// 当播放新的歌曲时，强制暂停上一首歌
}

// 停止播放
void McAudioPlayerWidget::stop() {
	d->m_mediaPlayer->stop();
}

void McAudioPlayerWidget::play() {
	d->m_mediaPlayer->play();
	d->m_isPaused = false;
}

void McAudioPlayerWidget::play(const QModelIndex &index) {
	d->playlistListView->setCurrentIndex(index);
	mcOutput << d->m_playlistModel->songName(index);
	QString url = d->m_playlistModel->songUrl(index);
	if (url.isEmpty()) {
		mcOutput << "歌曲链接未找到，歌曲失效";
		return;
	}
	play(url);
}

// 下一曲
void McAudioPlayerWidget::playNext(int index) {
	stop();
	int playIndex = getPlayModelIndex(index);
	replay(playIndex);
}

void McAudioPlayerWidget::replay(int index) {
	QModelIndex modelIndex = d->m_playlistModel->index(index);
	play(modelIndex);
}

void McAudioPlayerWidget::playPrevious() {
	int index = -1;
	replay(index);
}

int McAudioPlayerWidget::getPlayModelIndex(int index) {
	if (isExistsPlaylist(index))
		return index;

	int playNum = 0;
	switch (d->m_playMode)
	{
	case McAudioPlayerWidget::Shuffle:
		playNum = getRandomNum();
		break;
	case McAudioPlayerWidget::Repeat:
		playNum = d->playlistListView->currentIndex().row() + 1;
		playNum = playNum >= d->m_playlistModel->rowCount() ? 0 : playNum;
		break;
	case McAudioPlayerWidget::Single:
		playNum = d->playlistListView->currentIndex().row();
		break;
	default:
		break;
	}
	return playNum;
}

void McAudioPlayerWidget::togglePlayback() {
	if (d->m_mediaPlayer->mediaStatus() == QMediaPlayer::NoMedia) {
		int currIndex = d->playlistListView->currentIndex().row();
		isExistsPlaylist(currIndex) ? playNext(currIndex) : playNext();
	}
	else if (d->m_mediaPlayer->state() == QMediaPlayer::PlayingState) {
		d->m_isPaused = true;
		d->m_mediaPlayer->pause();
	}
	else {
		d->m_isPaused = false;
		d->m_mediaPlayer->play();
	}
}

bool McAudioPlayerWidget::isExistsPlaylist(int index) {
	if (index >= 0 && index < d->m_playlistModel->rowCount())
		return true;
	return false;
}

void McAudioPlayerWidget::updatePlayOrder() {
	switch (d->m_playMode)
	{
	case McAudioPlayerWidget::Shuffle:
		d->playOrderButton->setIcon(QIcon(":/McAudioPlayer/data/icon/shuffle.png"));
		d->playOrderButton->setAccessibleName("shuffle");
		break;
	case McAudioPlayerWidget::Repeat:
		d->playOrderButton->setIcon(QIcon(":/McAudioPlayer/data/icon/repeat.png"));
		d->playOrderButton->setAccessibleName("repeat");
		break;
	case McAudioPlayerWidget::Single:
		d->playOrderButton->setIcon(QIcon(":/McAudioPlayer/data/icon/single.png"));
		d->playOrderButton->setAccessibleName("single");
		break;
	default:
		break;
	}
	loadStyle(d->playOrderButton, ":/QSS/data/qss/PlayOrderButton.qss");
}

inline int McAudioPlayerWidget::getRandomNum() {
	int num = qrand() % d->m_playlistModel->rowCount();
	return num;
}

inline void McAudioPlayerWidget::setSongNum() {
	d->songNumLabel->setText(QString::number(d->m_songModel->rowCount()));
}

inline void McAudioPlayerWidget::loadSong(const QString &songlist) {
	d->m_songModel->loadSong(songlist);
	setSongNum();
}

void McAudioPlayerWidget::showMenu(QAbstractItemView *view, const QPoint &pos, QMenu *menu) {
	QItemSelectionModel *itemSelectionModel = view->selectionModel();
	if (!itemSelectionModel || !itemSelectionModel->hasSelection())
		return;
	if (itemSelectionModel->currentIndex() == view->indexAt(pos))
		menu->exec(QCursor::pos());
}

void McAudioPlayerWidget::disabledAddToMenu(const QString &text) {
	QList<QAction *> actions = d->m_addToMenu->actions();
	for (QAction *action : actions) {
		if (action->text() == text) {
			action->setEnabled(false);
			continue;
		}
		action->setEnabled(true);
	}
}

inline void McAudioPlayerWidget::addAddToMenu(const QString &text) {
	d->m_addToMenu->addAction(text);
}

void McAudioPlayerWidget::deleteAddToMenu(const QString &text) {
	QList<QAction *> actions = d->m_addToMenu->actions();
	for (QAction *action : actions) {
		if (action->text() == text) {
			d->m_addToMenu->removeAction(action);
			action->deleteLater();
			break;
		}
	}
}

void McAudioPlayerWidget::setPlaylistCount(int count) {
	QString countStr = "0  ";
	if (count > 0 && count < 10)
		countStr = QString::number(count) + "  ";
	else if (count >= 10 && count < 100)
		countStr = QString::number(count) + " ";
	else
		countStr = QString::number(count);
	d->playlistButton->setText(countStr);
}
