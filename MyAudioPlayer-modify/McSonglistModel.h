#pragma once

#include <qabstractitemmodel.h>
#include <qscopedpointer.h>

class McSonglistItem;

struct McSonglistModelData;

class McSonglistModel : public QAbstractListModel {
	Q_OBJECT

public:
	explicit McSonglistModel(QObject *parent = 0);
	~McSonglistModel();

	bool init();
	void clear();

	void loadSonglist();
	QString songlistName(const QModelIndex &index);
	QModelIndex addSonglist();
	bool isDeleteSonglist(const QModelIndex &index);
	void deleteSonglist(const QModelIndex &index);
	int songlistIndex(const QString &songlist);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:
	McSonglistItem *getItem(const QModelIndex &index) const;

private:
	QScopedPointer<McSonglistModelData> d;
};
