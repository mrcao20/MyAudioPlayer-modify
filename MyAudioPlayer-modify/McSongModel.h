#pragma once

#include "McTableModel.h"
#include <qscopedpointer.h>

struct McSongItem;

struct McSongModelData;

class McSongModel : public McTableModel {
	Q_OBJECT

public:
	explicit McSongModel(QObject *parent = 0);
	~McSongModel();

	bool init();
	void clear();

	void loadSong(const QString &songlist);

	int id(const QModelIndex &index);
	QVector<QVector<QString>> datas();
	void addTo(int songlistIndex, const QModelIndexList &indexList) override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:
	McSongItem *getItem(const QModelIndex &index) const;

private:
	QScopedPointer<McSongModelData> d;
};

