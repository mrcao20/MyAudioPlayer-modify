#pragma once

#include <QStyledItemDelegate>
#include <qdebug.h>
class McSonglistDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	explicit McSonglistDelegate(QObject *parent = 0);
	~McSonglistDelegate();

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};
