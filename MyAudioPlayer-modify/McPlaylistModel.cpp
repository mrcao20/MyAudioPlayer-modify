#include "McPlaylistModel.h"
#include <qcolor.h>
#include "McMusicMan.h"

struct McPlaylistModelData {
	McMusicMan *m_musicMan;
};

McPlaylistModel::McPlaylistModel(QObject *parent)
	: QAbstractListModel(parent)
	, d(new McPlaylistModelData)
{
}

McPlaylistModel::~McPlaylistModel(){
}

bool McPlaylistModel::init() {
	d->m_musicMan = new McMusicMan(this);
	if (!d->m_musicMan->init("audio_playlist_connection"))
		return false;
	d->m_musicMan->loadPlaylist();
	return true;
}

void McPlaylistModel::quit() {
	d->m_musicMan->exitAddPlaylist();
}

void McPlaylistModel::clear() {
	if (rowCount() <= 0)
		return;
	beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
	endRemoveRows();
	d->m_musicMan->clear();
}

bool McPlaylistModel::isExists(int id) {
	for (int i = 0; i < rowCount(); ++i) {
		McPlaylistItem *item = getItem(index(i));
		if (item && item->id == id)
			return true;
	}
	return false;
}

int McPlaylistModel::addSong(QVector<QVector<QString>> &datas) {
	clear();
	d->m_musicMan->addPlaylist(datas);
	if (rowCount() > 0) {
		beginInsertRows(QModelIndex(), 0, rowCount() - 1);
		endInsertRows();
	}
	return rowCount();
}

QString McPlaylistModel::songUrl(const QModelIndex &index) {
	QString url = "";
	if (!index.isValid())
		return url;
	McItemInterface *item = d->m_musicMan->item(index.row());
	url = item->url();
	return url;
}

QString McPlaylistModel::songName(const QModelIndex &index) {
	McPlaylistItem *item = getItem(index);
	if (!item)
		return "";
	return item->songTitle;
}

int McPlaylistModel::rowCount(const QModelIndex &parent) const {
	Q_UNUSED(parent);
	return d->m_musicMan->size();
}

QVariant McPlaylistModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();
	McPlaylistItem *playlistItem = getItem(index);
	if (playlistItem) {
		switch (role) {
		case Qt::DisplayRole:
			return playlistItem->songTitle + "\n" + playlistItem->artistsTitle;
		case Qt::TextAlignmentRole:
			return int(Qt::AlignLeft | Qt::AlignVCenter);
		}
	}
	return QVariant();
}

Qt::ItemFlags McPlaylistModel::flags(const QModelIndex &index) const {
	if (!index.isValid())
		return Qt::NoItemFlags;

	return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

McPlaylistItem *McPlaylistModel::getItem(const QModelIndex &index) const {
	McItemInterface *item = d->m_musicMan->item(index.row());
	if (item)
		return dynamic_cast<McPlaylistItem *>(item);
	return NULL;
}
