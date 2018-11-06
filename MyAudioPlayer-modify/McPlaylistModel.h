#pragma once

#include <qabstractitemmodel.h>
#include <qscopedpointer.h>

class McPlaylistItem;

struct McPlaylistModelData;

class McPlaylistModel : public QAbstractListModel {
	Q_OBJECT

public:
	explicit McPlaylistModel(QObject *parent = 0);
	~McPlaylistModel();

	bool init();
	void quit();
	void clear();

	bool isExists(int id);
	int addSong(QVector<QVector<QString>> &datas);
	QString songUrl(const QModelIndex &index);
	QString songName(const QModelIndex &index);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
	McPlaylistItem *getItem(const QModelIndex &index) const;

private:
	QScopedPointer<McPlaylistModelData> d;

};
