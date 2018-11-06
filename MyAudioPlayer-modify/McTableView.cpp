#include "McTableView.h"
#include <qheaderview.h>
#include <qevent.h>
#include "McTableModel.h"

struct McTableViewData {
	McTableModel *m_model;
	int m_currRow;
};

McTableView::McTableView(QWidget *parent)
	: QTableView(parent)
	, d(new McTableViewData)
{
	d->m_currRow = -1;
	d->m_model = NULL;

	horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);		 // 设置表头不能使用鼠标拖动
}

McTableView::~McTableView(){
}

void McTableView::setModel(McTableModel *model) {
	QTableView::setModel(model);
	d->m_model = model;
}

McTableModel *McTableView::model() {
	return d->m_model;
}

void McTableView::resizeEvent(QResizeEvent *event) {
	setColumnWidth(0, width() / 7 * 4);
	setColumnWidth(1, width() / 7);
	setColumnWidth(2, width() / 7 * 2 - 15);
}

void McTableView::mouseMoveEvent(QMouseEvent * event) {
	int row = indexAt(event->pos()).row();
	updateRow(row);
	QTableView::mouseMoveEvent(event);
}

void McTableView::leaveEvent(QEvent * event) {
	if (!d->m_model)
		return;
	d->m_model->setHoverRow(-1);
	int column_count = model()->columnCount();
	for (int i = column_count - 1; i >= 0; i--)
	{
		update(model()->index(d->m_currRow, i));
	}
	d->m_currRow = -1;
	QTableView::leaveEvent(event);
}

void McTableView::updateRow(int row) {
	if (row == d->m_currRow)
		return;

	if (!d->m_model)
		return;
	d->m_model->setHoverRow(row);
	int column_count = model()->columnCount();
	for (int i = column_count - 1; i >= 0; i--)
	{
		update(model()->index(d->m_currRow, i));
		update(model()->index(row, i));
	}
	d->m_currRow = row;
}
