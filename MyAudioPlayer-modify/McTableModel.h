#pragma once

#include <QAbstractTableModel>

class McTableModel : public QAbstractTableModel {
	Q_OBJECT

public:
	explicit McTableModel(QObject *parent = 0)
		: QAbstractTableModel(parent)
		, m_hoverRow(-1){}
	virtual ~McTableModel() = default;

	void setHoverRow(int hoverRow){ m_hoverRow = hoverRow; }
	virtual void addTo(int songlistIndex, const QModelIndexList &indexList) = 0;

protected:
	void clearModel() {
		if (rowCount() <= 0)
			return;
		beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
		endRemoveRows();
	}

protected:
	int m_hoverRow;
};
