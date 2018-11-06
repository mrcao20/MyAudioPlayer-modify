#pragma once

#include <QTableView>
#include <qscopedpointer.h>

QT_BEGIN_NAMESPACE
class QResizeEvent;
class QMouseEvent;
class QEvent;
QT_END_NAMESPACE

class McTableModel;

struct McTableViewData;

class McTableView : public QTableView{
	Q_OBJECT

public:
	explicit McTableView(QWidget *parent = 0);
	~McTableView();

	void setModel(McTableModel *model);
	McTableModel *model();

protected:
	void resizeEvent(QResizeEvent *event) override;
	void mouseMoveEvent(QMouseEvent * event) override;
	void leaveEvent(QEvent * event) override;

private:
	void updateRow(int row);

private:
	QScopedPointer<McTableViewData> d;
};
