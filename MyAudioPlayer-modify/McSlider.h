#pragma once

#include <qslider.h>
#include <qscopedpointer.h>

struct McSliderData;

class McSlider : public QSlider{
	Q_OBJECT

public:
	explicit McSlider(QWidget *parent = 0);
	virtual ~McSlider() = default;

signals:
	void showCurrPos(int pos);
	void cursorLeave();

protected:
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void leaveEvent(QEvent *event) override;

private:
	int getCursorCurrentPos();

private:
	QScopedPointer<McSliderData> d;
};
