#include "McSongModel.h"
#include <qcolor.h>
#include "McMusicMan.h"
#include "McGlobal.h"

struct McSongModelData {
	McMusicMan *m_musicMan;
	QStringList m_hHeaders;
};

McSongModel::McSongModel(QObject *parent)
	: McTableModel(parent)
	, d(new McSongModelData)
{
	
}

McSongModel::~McSongModel(){
}

bool McSongModel::init() {
	d->m_hHeaders << "歌名" << "歌手" << "专辑";
	d->m_musicMan = new McMusicMan(this);
	return d->m_musicMan->init("audio_song_connection");
}

void McSongModel::clear() {
	clearModel();
	d->m_musicMan->clear();
}

void McSongModel::loadSong(const QString &songlist) {
	clear();
	d->m_musicMan->loadSong(songlist);
	if (rowCount() <= 0)
		return;
	beginInsertRows(QModelIndex(), 0, rowCount() - 1);
	endInsertRows();
}

int McSongModel::id(const QModelIndex &index) {
	int id = -1;
	McSongItem *item = getItem(index);
	if (!item)
		return id;
	id = item->id;
	return id;
}

QVector<QVector<QString>> McSongModel::datas() {
	QVector<QVector<QString>> data;
	for (int i = 0; i < rowCount(); ++i) {
		McSongItem *item = getItem(index(i, 0));
		if (!item)
			continue;
		data << (QVector<QString>() << QString::number(item->id) << QString::number(item->songIndex) << item->songId << item->songMid 
			<< item->songTitle << item->songSrc << item->songUrl << item->artistsTitle);
	}
	return data;
}

void McSongModel::addTo(int songlistIndex, const QModelIndexList &indexList) {
	for (const QModelIndex &index : indexList) {
		if (!index.isValid())
			continue;
		McItemInterface *item = d->m_musicMan->item(index.row());
		if (item && !item->addTo(songlistIndex)) {	// 如果item存在，则添加，不存在则进入下一个循环，不执行添加语句
			mcOutput << "歌曲添加失败";
		}
	}
}

int McSongModel::rowCount(const QModelIndex &parent) const {
	// parent指父索引，一般情况下table的parent都无效，表示没有父索引，但是如果存在，行数量需返回0
	return (parent.isValid()) ? 0 : d->m_musicMan->size();
}

int McSongModel::columnCount(const QModelIndex &parent) const {
	return (parent.isValid()) ? 0 : 3;
}

QVariant McSongModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();
	int column = index.column();
	int row = index.row();  //行号
	QString displayStr = "";
	McSongItem *songItem = getItem(index);
	if (songItem) {
		switch (column) {
		case 0:
			displayStr = songItem->songTitle;
			break;
		case 1:
			displayStr = songItem->artistsTitle;
			break;
		case 2:
			displayStr = songItem->albumTitle;
			break;
		}
	}
	switch (role) {
	case Qt::DisplayRole:
		return displayStr;
	case Qt::TextAlignmentRole:
		return int(Qt::AlignLeft | Qt::AlignVCenter);
	case Qt::BackgroundRole:
		if (row == m_hoverRow) {
			return QColor(28, 153, 62);
		}
	}
	return QVariant();
}

QVariant McSongModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		if (section >= d->m_hHeaders.size())
			return QVariant();
		else
			return d->m_hHeaders.at(section);
	}
	if (role == Qt::TextAlignmentRole && orientation == Qt::Horizontal) {
		return int(Qt::AlignLeft | Qt::AlignVCenter);
	}
	return QVariant();
}

Qt::ItemFlags McSongModel::flags(const QModelIndex &index) const {
	if (!index.isValid())
		return Qt::NoItemFlags;

	return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool McSongModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	return McTableModel::setData(index, value, role);
}

McSongItem *McSongModel::getItem(const QModelIndex &index) const {
	McItemInterface *item = d->m_musicMan->item(index.row());
	if (item)
		return dynamic_cast<McSongItem *>(item);
	return NULL;
}
