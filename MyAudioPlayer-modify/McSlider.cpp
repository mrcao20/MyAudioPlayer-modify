#include "McSlider.h"
#include <qevent.h>

struct McSliderData {
	bool m_isPress;
};

McSlider::McSlider(QWidget *parent)
	: QSlider(parent) 
	, d(new McSliderData)
{
	setMouseTracking(true);
	d->m_isPress = false;
}

int McSlider::getCursorCurrentPos() {
	QPoint mousePoint = parentWidget()->mapFromGlobal(QCursor::pos());
	int position = (mousePoint.x() - x()) / (width() * 1.0) * maximum();
	return position;
}

void McSlider::mouseMoveEvent(QMouseEvent *event) {
	int pos = getCursorCurrentPos();
	emit showCurrPos(pos);
	if (d->m_isPress) {
		setValue(pos);
		return;
	}
	QSlider::mouseMoveEvent(event);
}

void McSlider::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		int pos = getCursorCurrentPos();
		setValue(pos);
		d->m_isPress = true;
		return;
	}
	QSlider::mousePressEvent(event);
}

void McSlider::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		d->m_isPress = false;
		return;
	}
	QSlider::mouseReleaseEvent(event);
}

void McSlider::leaveEvent(QEvent *event) {
	emit cursorLeave();
	QSlider::leaveEvent(event);
}
