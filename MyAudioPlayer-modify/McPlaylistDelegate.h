#pragma once

#include <QStyledItemDelegate>
#include <qscopedpointer.h>

struct McPlaylistDelegateData;

class McPlaylistDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	enum ButtonState {
		Normal,
		Hover,
		Press
	};

	explicit McPlaylistDelegate(QObject *parent = 0);
	~McPlaylistDelegate();

	void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const override;
	bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;

signals:
	void play(const QPoint &pos, const QModelIndex &index);
	void toPosition(const QPoint &pos, const QModelIndex &index);
	void more(const QPoint &pos, const QModelIndex &index);

private:
	inline void showToolTip(const QString &text) const;

private:
	QScopedPointer<McPlaylistDelegateData> d;
};
