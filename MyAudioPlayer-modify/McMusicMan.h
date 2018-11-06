#pragma once

#include <QObject>
#include <qscopedpointer.h>
#include <qvector.h>

class McItemInterface {
public:
	virtual ~McItemInterface() = default;

	virtual bool rename(const QString &newName) = 0;
	virtual bool itemDelete() = 0;
	virtual bool addTo(int songlistIndex) = 0;
	virtual QString url() = 0;
};

class McAbstractItem : public McItemInterface {
public:
	virtual ~McAbstractItem() = default;

	// 当子类不实现接口，则直接返回true表示成功
	bool rename(const QString &newName) override { return true; }
	bool itemDelete() override { return true; }
	bool addTo(int songlistIndex) override { return true; }
	QString url() override { return ""; }
};

struct McMusicManData;

class McMusicMan : public QObject{
	Q_OBJECT

public:
	explicit McMusicMan(QObject *parent = 0);
	~McMusicMan();

	bool init(const QString &dbConnectionName);
	void exitAddPlaylist();
	void clear();

	int size();
	McItemInterface *item(int i);
	void removeItem(McItemInterface *item);
	void loadSonglist();
	void loadSong(const QString &songlist);
	void loadPlaylist();
	bool deleteSonglist(const QString &songlist);
	void addSonglist();
	bool addSong(int songlistIndex, int songIndex);
	QString url(const QString &songId, const QString &src);
	void updateUrl(int songIndex, const QString &url);
	void addPlaylist(QVector<QVector<QString>> &datas);
	int songlistIndex(const QString &songlist);
	bool renameSonglist(const QString &newSonglist, const QString &oldSonglist);

signals:
	void addPlaylistFinished();

private:
	int getMaxId(const QString &view);
	QString getNewSonglist();
	QString getView(const QString &table);
	void resetAutoIncrement(const QString &table);
	void addPlaylist();
	
private:
	QScopedPointer<McMusicManData> d;
};

class McSonglistItem : public McAbstractItem {
public:
	McSonglistItem(McMusicMan *musicMan) : m_musicMan(musicMan), songlistIndex(-1), songlistName("") {}
	McSonglistItem(McMusicMan *musicMan, int songlistIndex, const QString &songlistName)
		: m_musicMan(musicMan), songlistIndex(songlistIndex), songlistName(songlistName) {}

	int songlistIndex;
	QString songlistName;

	inline bool rename(const QString &newName) override {
		if (!m_musicMan)
			return false;
		if (m_musicMan->renameSonglist(newName, songlistName)) {
			songlistName = newName;
			return true;
		}
		return false;
	}

	inline bool itemDelete() override {
		if (!m_musicMan)
			return false;
		return m_musicMan->deleteSonglist(songlistName);
	}

private:
	McMusicMan *m_musicMan;
};

class McSongItem : public McAbstractItem {
public:
	McSongItem(McMusicMan *musicMan) : m_musicMan(musicMan), id(-1), songIndex(-1), songId(""), songMid(""), songTitle(""),
		songSrc(""), songUrl(""), albumTitle(""), artistsTitle("") {}
	McSongItem(McMusicMan *musicMan, int id, int songIndex, const QString &songId, const QString &songMid, const QString &songTitle, 
		const QString &songSrc, const QString &songUrl,const QString &albumTitle, const QString &artistsTitle)
		: m_musicMan(musicMan), id(id), songIndex(songIndex), songId(songId), songMid(songMid), songTitle(songTitle), songSrc(songSrc)
		, songUrl(songUrl), albumTitle(albumTitle), artistsTitle(artistsTitle) {}

	inline void addArtistsTitle(const QString &title) {
		artistsTitle += ("/" + title);
	}

	int id;
	int songIndex;
	QString songId;
	QString songMid;
	QString songTitle;
	QString songSrc;
	QString songUrl;
	QString albumTitle;
	QString artistsTitle;

	bool addTo(int songlistIndex) override {
		if (!m_musicMan)
			return false;
		return m_musicMan->addSong(songlistIndex, songIndex);
	}

private:
	McMusicMan *m_musicMan;
};

class McPlaylistItem : public McAbstractItem {
public:
	McPlaylistItem(McMusicMan *musicMan) : m_musicMan(musicMan), playlistIndex(-1), id(-1), songId(""), songMid(""), songTitle(""),
		songSrc(""), songUrl(""), artistsTitle("") {}
	McPlaylistItem(McMusicMan *musicMan, int playlistIndex, int id, int songIndex, const QString &songId, const QString &songMid,
		const QString &songTitle, const QString &songSrc,const QString &songUrl, const QString &artistsTitle)
		: m_musicMan(musicMan), playlistIndex(playlistIndex), id(id), songIndex(songIndex), songId(songId), songMid(songMid), songTitle(songTitle)
		, songSrc(songSrc), songUrl(songUrl), artistsTitle(artistsTitle) {}

	inline QString url() override {
		QString url = songUrl;
		if (url.isEmpty()) {
			QString id = "";
			if (songSrc == "qqmusic")
				id = songMid;
			else if (songSrc == "ymusic")
				id = songId;
			url = m_musicMan->url(id, songSrc);
			songUrl = url;
			m_musicMan->updateUrl(songIndex, songUrl);
		}
		return url;
	}

	int playlistIndex;
	int id;
	int songIndex;
	QString songId;
	QString songMid;
	QString songTitle;
	QString songSrc;
	QString songUrl;
	QString artistsTitle;

private:
	McMusicMan *m_musicMan;
};
