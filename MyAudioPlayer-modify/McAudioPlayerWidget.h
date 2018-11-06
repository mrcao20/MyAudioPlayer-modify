#pragma once

#include <QObject>
#include <qscopedpointer.h>

QT_BEGIN_NAMESPACE
class QMenu;
class QListWidget;
class QAbstractItemView;
QT_END_NAMESPACE

struct McAudioPlayerWidgetData;

class McAudioPlayerWidget : public QObject{
	Q_OBJECT

public:
	enum PlayMode {
		Shuffle,
		Repeat,
		Single
	};

	explicit McAudioPlayerWidget(QObject *parent = Q_NULLPTR);
	~McAudioPlayerWidget();

	bool init(QWidget *w);

	void play(const QString &link);
	void pause();
	void stop();

private:
	void loadStyle(QWidget *widget, const QString &fileName);
	void play();
	void play(const QModelIndex &index);
	void playNext(int index = -1);
	void replay(int index = -1);
	void playPrevious();
	int getPlayModelIndex(int index = -1);
	void togglePlayback();
	bool isExistsPlaylist(int index);
	void updatePlayOrder();
	inline int getRandomNum();
	inline void setSongNum();
	inline void loadSong(const QString &songlist);
	void showMenu(QAbstractItemView *view, const QPoint &pos, QMenu *menu);
	void disabledAddToMenu(const QString &text);
	inline void addAddToMenu(const QString &text);
	void deleteAddToMenu(const QString &text);
	void setPlaylistCount(int count);

private:
	QScopedPointer<McAudioPlayerWidgetData> d;
};
