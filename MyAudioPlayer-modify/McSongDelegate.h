#pragma once

#include <QStyledItemDelegate>
#include <qscopedpointer.h>

struct McSongDelegateData;

class McSongDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	enum ButtonState {
		Normal,
		Hover,
		Press
	};

	explicit McSongDelegate(QObject *parent = 0);
	~McSongDelegate();

	void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const override;
	bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;

signals:
	void play(const QPoint &pos, const QModelIndex &index);
	void addTo(const QPoint &pos, const QModelIndex &index);
	void download(const QPoint &pos, const QModelIndex &index);
	void more(const QPoint &pos, const QModelIndex &index);

private:
	inline void showToolTip(const QString &text) const;

private:
	QScopedPointer<McSongDelegateData> d;
};
