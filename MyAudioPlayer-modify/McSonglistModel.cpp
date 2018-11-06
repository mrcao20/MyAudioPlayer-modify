#include "McSonglistModel.h"
#include <qmessagebox.h>
#include "McMusicMan.h"
#include "McGlobal.h"

struct McSonglistModelData {
	McMusicMan *m_musicMan;
};

McSonglistModel::McSonglistModel(QObject *parent)
	: QAbstractListModel(parent)
	, d(new McSonglistModelData)
{
}

McSonglistModel::~McSonglistModel(){
}

bool McSonglistModel::init() {
	d->m_musicMan = new McMusicMan(this);
	return d->m_musicMan->init("audio_songlist_connection");
}

void McSonglistModel::clear() {
	if (rowCount() <= 0)
		return;
	beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
	endRemoveRows();
	d->m_musicMan->clear();
}

void McSonglistModel::loadSonglist() {
	clear();
	d->m_musicMan->loadSonglist();
	if (rowCount() <= 0)
		return;
	beginInsertRows(QModelIndex(), 0, rowCount() - 1);
	endInsertRows();
}

QString McSonglistModel::songlistName(const QModelIndex &index) {
	if (!index.isValid())
		return "";
	McSonglistItem *item = getItem(index);
	if (!item)
		return "";
	return item->songlistName;
}

QModelIndex McSonglistModel::addSonglist() {
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	d->m_musicMan->addSonglist();
	endInsertRows();
	return index(rowCount() - 1);
}

bool McSonglistModel::isDeleteSonglist(const QModelIndex &index) {
	if (rowCount() <= 1 || index.row() == 0)
		return false;
	QString songlist = songlistName(index);
	int ret = QMessageBox::warning(NULL, "Confirm delete",
		QString("are you sure you want to delete this songlist named %1?").arg(songlist),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	if (ret == QMessageBox::No)
		return false;
	return true;
}

void McSonglistModel::deleteSonglist(const QModelIndex &index) {
	if (!index.isValid())
		return;
	McItemInterface *item = d->m_musicMan->item(index.row());
	if (!item)
		return;
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	item->itemDelete();
	d->m_musicMan->removeItem(item);
	endRemoveRows();
	mcOutput << "删除完成";
}

int McSonglistModel::songlistIndex(const QString &songlist) {
	return d->m_musicMan->songlistIndex(songlist);
}

int McSonglistModel::rowCount(const QModelIndex &parent) const {
	return d->m_musicMan->size();
}

QVariant McSonglistModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();
	McSonglistItem *songlistItem = getItem(index);
	if (songlistItem) {
		switch (role) {
		case Qt::DisplayRole:
			return songlistItem->songlistName;
		case Qt::TextAlignmentRole:
			return int(Qt::AlignLeft | Qt::AlignVCenter);
		case Qt::EditRole:
			return songlistItem->songlistName;
		}
	}
	return QVariant();
}

Qt::ItemFlags McSonglistModel::flags(const QModelIndex &index) const {
	if (!index.isValid())
		return Qt::NoItemFlags;
	
	return  Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool McSonglistModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (!index.isValid()){
		return false;
	}

	if (role == Qt::EditRole){
		McSonglistItem *songlistItem = getItem(index);
		if (!songlistItem || value.toString() == songlistItem->songlistName)
			return false;
		if (!songlistItem->rename(value.toString())) {
			Mc::showWarning("歌单名重复，重新输入！");
			return false;
		}
		emit(dataChanged(index, index));
		return true;
	}
	return false;
}

McSonglistItem *McSonglistModel::getItem(const QModelIndex &index) const {
	McItemInterface *item = d->m_musicMan->item(index.row());
	if (item)
		return dynamic_cast<McSonglistItem *>(item);
	return NULL;
}
