/*	MySQL�е���ͼΪ�ɸ�����ͼ����������ͼ��Ϣ������
	����SQLite�е���ͼΪֻ���Ĳ��ɸ�����ͼ��Ϊ���ּ�����
	�����ݵĸ��ºͲ���Ȳ�����������ͼ�н��У�ȫ��ʹ��ԭʼ��
*/

#include "McMusicMan.h"
#include <QtConcurrent>
#include <qeventloop.h>
#include <qscopedpointer.h>
#include "McDBTools.h"
#include "McGlobal.h"
#include "McMusicApiInterface.h"
#include "McMusicApiFactory.h"

struct McMusicManData {
	McDBTools *m_dbTools;
	QList<McItemInterface *> m_items;
	bool m_isAddplaylistRunning;
	bool m_isExitThread;
	QScopedPointer<McMusicApiInterface> m_musicApi;
};

McMusicMan::McMusicMan(QObject *parent)
	: QObject(parent)
	, d(new McMusicManData)
{
	d->m_isAddplaylistRunning = false;
	d->m_isExitThread = false;
}

McMusicMan::~McMusicMan(){
	clear();
}

bool McMusicMan::init(const QString &dbConnectionName) {
	d->m_dbTools = new McDBTools(dbConnectionName, this);
	return d->m_dbTools->init();
}

void McMusicMan::exitAddPlaylist() {
	if (d->m_isAddplaylistRunning) {
		d->m_isExitThread = true;
		QEventLoop loop;
		connect(this, &McMusicMan::addPlaylistFinished, &loop, &QEventLoop::quit);
		loop.exec();
		d->m_isAddplaylistRunning = false;
		d->m_isExitThread = false;
	}
}

void McMusicMan::clear() {
	Mc::clearItem(d->m_items);
}

int McMusicMan::size() {
	return d->m_items.size();
}

McItemInterface *McMusicMan::item(int i) {
	return Mc::exists(d->m_items, i) ? d->m_items.at(i) : NULL;
}

void McMusicMan::removeItem(McItemInterface *item) {
	Mc::remove(d->m_items, item);
}

void McMusicMan::loadSonglist() {
	if (!d->m_dbTools->readFromDB(QVector<QString>() << "songlist_index" << "songlist_name", "t_songlist_detail_info")) {
		mcOutput << "�޷������ݿ��ж�ȡ����";
		return;
	}
	// �����֮ǰ�����ݣ����¼���
	clear();
	// ����next������ֱ���ж�record�ķ���ֵ�Ƿ�Ϊ�գ������next����true��record������Ҫ������Ŀ����ж�
	while (d->m_dbTools->next()) {
		QVector<QString> data = d->m_dbTools->record();
		bool ok = false;
		int index = data.at(0).toInt(&ok);
		if (!ok) {
			mcOutput << "��������";
			return;
		}
		d->m_items << new McSonglistItem(this, index, data.at(1));
	}
}

void McMusicMan::loadSong(const QString &songlist) {
	if (!d->m_dbTools->readFromDB(QVector<QString>() << "id" << "song_index" << "song_id" << "song_mid" << "song_title" 
		<< "song_src" << "song_url" << "album_title" << "artists_title", "v_music_info", "songlist_name", songlist)) {
		mcOutput << "�޷������ݿ��ж�ȡ����";
		return;
	}
	// ����next������ֱ���ж�record�ķ���ֵ�Ƿ�Ϊ�գ������next����true��record������Ҫ������Ŀ����ж�
	clear();
	int lastId = -1;
	McSongItem *lastItem = NULL;
	while (d->m_dbTools->next()) {
		QVector<QString> data = d->m_dbTools->record();
		bool ok = false;
		bool ok2 = false;
		int id = data.at(0).toInt(&ok);
		int songIndex = data.at(1).toInt(&ok2);
		if (!ok || !ok2) {
			mcOutput << "��������";
			return;
		}
		if (lastId == id && lastItem) {
			lastItem->addArtistsTitle(data.at(8));
			continue;
		}
		
		lastId = id;
		lastItem = new McSongItem(this, id, songIndex, data.at(2), data.at(3), data.at(4), data.at(5), data.at(6), data.at(7), data.at(8));
		d->m_items.append(lastItem);
	}
	// ��ת�����ݿ��и����Ĵ洢˳�����ʾ˳���෴
	std::reverse(d->m_items.begin(), d->m_items.end());
}

void McMusicMan::loadPlaylist() {
	if (!d->m_dbTools->readFromDB(QVector<QString>() << "playlist_index" << "id" << "song_index" << "song_id" << "song_mid" << "song_title" 
		<< "song_src" << "song_url" << "artists_title", "t_playlist_info")) {
		mcOutput << "�޷������ݿ��ж�ȡ����";
		return;
	}
	clear();
	while (d->m_dbTools->next()) {
		QVector<QString> data = d->m_dbTools->record();
		bool ok = false;
		bool ok2 = false;
		bool ok3 = false;
		int playlistIndex = data.at(0).toInt(&ok);
		int id = data.at(1).toInt(&ok2);
		int songIndex = data.at(2).toInt(&ok3);
		if (!ok || !ok2 || !ok3) {
			mcOutput << "��������";
			return;
		}
		d->m_items << new McPlaylistItem(this, playlistIndex, id, songIndex, data.at(3), data.at(4), data.at(5), data.at(6), data.at(7), data.at(8));
	}
}

bool McMusicMan::deleteSonglist(const QString &songlist) {
	if (!d->m_dbTools->deleteFromDB("t_songlist_detail_info", "songlist_name", songlist)) {
		mcOutput << "�赥ɾ��ʧ��";
		return false;
	}
	resetAutoIncrement("t_songlist_detail_info");
	resetAutoIncrement("t_music_info");
	resetAutoIncrement("t_artists_detail_info");
	resetAutoIncrement("t_album_detail_info");
	mcOutput << "�赥ɾ���ɹ�";
	return true;
}

QString McMusicMan::getNewSonglist() {
	QString songlist = "new songlist";
	int i = 0;
	do {
		if (i > 0)
			songlist = "new songlist" + QString::number(i);
		if (!d->m_dbTools->readFromDB("*", "t_songlist_detail_info", "songlist_name", songlist))
			break;
		i++;
	} while (d->m_dbTools->next());
	return songlist;
}

void McMusicMan::addSonglist() {
	QString songlist = getNewSonglist();
	// getNewSonglist��ѭ���жϵõ�һ�����ݿ��в����ڵĸ赥���ʴ˴������ٴ����ж�
	d->m_dbTools->insertIntoDB("t_songlist_detail_info", "songlist_name", songlist);
	// ������Ӹ赥ʱ����ָ��Ψһ��ʶ������ʶ��Ϊ�������룬����ӳɹ�֮��赥������ֵ��Ϊ��ǰ�¸赥�ı�ʶ��
	int songlistIndex = getMaxId("v_max_songlist_index");
	d->m_items << new McSonglistItem(this, songlistIndex, songlist);
}

bool McMusicMan::addSong(int songlistIndex, int songIndex) {
	int id = getMaxId("v_max_id") + 1;	// ��ȡ��id
	return d->m_dbTools->insertIntoDB("t_songlist_info", QVector<QString>() << "id" << "songlist_index" << "song_index"
		, QVector<QString>() << QString::number(id) << QString::number(songlistIndex) << QString::number(songIndex));
}

QString McMusicMan::url(const QString &songId, const QString &src) {
	QString url = "";
	if (songId.isEmpty())
		return url;
	McMusicApiFactory factory;
	d->m_musicApi.reset(factory.getMusicApi(src));
	url = d->m_musicApi->getSongUrl(songId);
	return url;
}

// ���¸�����url������
void McMusicMan::updateUrl(int songIndex, const QString &url) {
	if (url.isEmpty())
		return;
	d->m_dbTools->updateDB("t_playlist_info", "song_url", url, "song_index", QString::number(songIndex));
	d->m_dbTools->updateDB("t_music_info", "song_url", url, "song_index", QString::number(songIndex));
}

void McMusicMan::addPlaylist(QVector<QVector<QString>> &datas) {
	// ���item
	clear();
	int playlistIndex = 1;
	for (QVector<QString> &data : datas) {
		if (data.size() != 8)
			return;
		bool ok = false;
		bool ok2 = false;
		int id = data.at(0).toInt(&ok);
		int songIndex = data.at(1).toInt(&ok2);
		if (!ok || !ok2)
			return;
		d->m_items << new McPlaylistItem(this, playlistIndex++, id, songIndex, data.at(2), data.at(3), data.at(4), data.at(5), data.at(6), data.at(7));
	}
	exitAddPlaylist();
	QtConcurrent::run(this, &McMusicMan::addPlaylist);
}

int McMusicMan::songlistIndex(const QString &songlist) {
	int index = -1;
	for (McItemInterface *item : d->m_items) {
		McSonglistItem *songlistItem = dynamic_cast<McSonglistItem *>(item);
		if (songlistItem && songlistItem->songlistName == songlist) {
			index = songlistItem->songlistIndex;
			break;
		}
	}
	return index;
}

bool McMusicMan::renameSonglist(const QString &newSonglist, const QString &oldSonglist) {
	bool isRename = true;
	if (!d->m_dbTools->readFromDB("*", "t_songlist_detail_info", "songlist_name", newSonglist)) {
		mcOutput << "�޷������ݿ��ж�ȡ����";
		return false;
	}
	// ��������ݿ����ܹ����ҵ��ø赥���򷵻�false���޷�����
	isRename = !d->m_dbTools->next();
	if (isRename)
		isRename = d->m_dbTools->updateDB("t_songlist_detail_info", "songlist_name", newSonglist, "songlist_name", oldSonglist);
	return isRename;
}

int McMusicMan::getMaxId(const QString &view) {
	int maxId = -1;
	if (view.isEmpty())
		return maxId;
	if (!d->m_dbTools->readFromDB("*", view)) {
		mcOutput << "�޷������ݿ��ж�ȡ����";
		return maxId;
	}
	// ����next������ֱ���ж�record�ķ���ֵ�Ƿ�Ϊ�գ������next����true��record������Ҫ������Ŀ����ж�
	if (!d->m_dbTools->next())
		return maxId;
	bool ok = false;
	int temp = d->m_dbTools->record().at(0).toInt(&ok);
	if (ok)
		maxId = temp;
	return maxId;
}

QString McMusicMan::getView(const QString &table) {
	if (table == "t_songlist_detail_info")
		return "v_max_songlist_index";
	else if (table == "t_music_info")
		return "v_max_song_index";
	else if (table == "t_artists_detail_info")
		return "v_max_artists_index";
	else if (table == "t_album_detail_info")
		return "v_max_album_index";
	else
		return "";
}

void McMusicMan::resetAutoIncrement(const QString &table) {
	QString view = getView(table);
	int maxId = getMaxId(view);
	if (maxId == -1)	// �������-1�����ʾ��ѯʧ�ܣ�ֱ�ӷ���
		return;
	// �˴�Ҳ��ʹ��maxId+1����ʾ�������������Ϊ��ǰ���һ����¼֮�󣬺ͽ��������Ϊ���һ����¼Ч��һ��
	d->m_dbTools->setAutoIncrement(table, maxId);
}

void McMusicMan::addPlaylist() {
	McDBTools dbTool("audio_playlist_connection_temp");
	dbTool.init();
	/*	������������б���������������
		ʹ��||���������ִ�е�һ����䣬�������false��ֱ��return������ִ�еڶ�����䣬
		�������true����һ��������������ִ�еڶ�����䣬���false�򷵻أ���֮����ִ��
	*/
	if (!dbTool.deleteFromDB("t_playlist_info") || !dbTool.setAutoIncrement("t_playlist_info", 1))
		return;
	d->m_isAddplaylistRunning = true;
	for (McItemInterface *item : d->m_items) {
		if (d->m_isExitThread) {
			emit addPlaylistFinished();
			break;
		}
		McPlaylistItem *playlistItem = dynamic_cast<McPlaylistItem *>(item);
		if (!item)
			break;
		if (!dbTool.insertIntoDB("t_playlist_info", QVector<QString>() << "id" << "song_index" << "song_id" << "song_mid"
			<< "song_title" << "song_src" << "song_url" << "artists_title",
			QVector<QString>() << QString::number(playlistItem->id) << QString::number(playlistItem->songIndex) << playlistItem->songId 
			<< playlistItem->songMid << playlistItem->songTitle << playlistItem->songSrc << playlistItem->songUrl << playlistItem->artistsTitle))
			break;
	}
	d->m_isAddplaylistRunning = false;
}
