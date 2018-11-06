#include "McPlaylistDelegate.h"
#include <qpushbutton.h>
#include <qevent.h>
#include <qapplication.h>
#include <qtooltip.h>
#include <qpainter.h>
#include "McGlobal.h"

struct McPlaylistDelegateData {
	QPoint m_mousePoint;  // 鼠标位置
	QVector<QPushButton *> m_pushButtons;
	QVector<QString> m_pushButtonTexts;
	QVector<std::function<void(const QPoint &, const QModelIndex &)>> m_buttonFuncs;
	int m_spacing;  // 按钮之间的间距
	int m_width;  // 按钮宽度
	int m_height;  // 按钮高度
	McPlaylistDelegate::ButtonState m_type;  // 按钮状态-1：划过 2：按下
};

McPlaylistDelegate::McPlaylistDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
	, d(new McPlaylistDelegateData)
{
	d->m_pushButtons.append(new QPushButton());
	d->m_pushButtons.append(new QPushButton());
	d->m_pushButtons.append(new QPushButton());
	d->m_pushButtonTexts << "播放" << "定位"  << "更多";
	d->m_buttonFuncs.append(std::bind(&McPlaylistDelegate::play, this, std::placeholders::_1, std::placeholders::_2));
	d->m_buttonFuncs.append(std::bind(&McPlaylistDelegate::toPosition, this, std::placeholders::_1, std::placeholders::_2));
	d->m_buttonFuncs.append(std::bind(&McPlaylistDelegate::more, this, std::placeholders::_1, std::placeholders::_2));
	d->m_spacing = 5;
	d->m_width = 25;
	d->m_height = 20;
	d->m_type = ButtonState::Normal;
	d->m_mousePoint = QPoint(-1, -1);

	// 设置按钮正常、划过、按下样式
	d->m_pushButtons.at(0)->setStyleSheet("QPushButton {border: none; background-color: transparent; image:url(:/McAudioPlayer/data/icon/playButton.png);} \
                                QPushButton:hover {image:url(:/McAudioPlayer/data/icon/playButton_hover.png);}	\
								QPushButton:pressed {image:url(:/McAudioPlayer/data/icon/playButton_hover.png);}");

	d->m_pushButtons.at(1)->setStyleSheet("QPushButton {border: none; background-color: transparent; image:url(:/McAudioPlayer/data/icon/addTo.png);} \
                                QPushButton:hover {image:url(:/McAudioPlayer/data/icon/addTo_hover.png);}	\
								QPushButton:pressed {image:url(:/McAudioPlayer/data/icon/addTo_hover.png);}");

	d->m_pushButtons.at(2)->setStyleSheet("QPushButton {border: none; background-color: transparent; image:url(:/McAudioPlayer/data/icon/more.png);} \
                                QPushButton:hover {image:url(:/McAudioPlayer/data/icon/more_hover.png);}	\
								QPushButton:pressed {image:url(:/McAudioPlayer/data/icon/more_hover.png);}");
}

McPlaylistDelegate::~McPlaylistDelegate(){
}

void McPlaylistDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
	QStyleOptionViewItem viewOption(option);
	initStyleOption(&viewOption, index);

	if (option.state.testFlag(QStyle::State_HasFocus))
		// 进行亦或操作，从state中去除State_HasFocus状态
		viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

	QStyledItemDelegate::paint(painter, viewOption, index);
	
	if (option.state.testFlag(QStyle::State_MouseOver) || option.state.testFlag(QStyle::State_Selected)) {
		for (int i = 0; i < d->m_pushButtons.size(); ++i) {
			// 绘制按钮
			QStyleOptionButton button;
			// 当前项最右边的点减去按钮宽度加间隔乘以剩余按钮的数量
			int x = (option.rect.width() + option.rect.x()) - (d->m_pushButtons.size() - i) * (d->m_width + d->m_spacing);
			// 当前项的y值加上当前项的高度减按钮宽度除以二
			int y = option.rect.y() + (option.rect.height() - d->m_height) / 2;
			button.rect = QRect(x, y, d->m_width, d->m_height);
			button.state.setFlag(QStyle::State_Enabled);
			if (button.rect.contains(d->m_mousePoint)) {
				if (d->m_type == ButtonState::Hover) {
					button.state.setFlag(QStyle::State_MouseOver);
				}
				else if (d->m_type == ButtonState::Press) {
					button.state.setFlag(QStyle::State_Sunken);
				}
			}

			QWidget *widget = d->m_pushButtons.at(i);
			widget->style()->drawControl(QStyle::CE_PushButton, &button, painter, widget);
		}
	}
}

bool McPlaylistDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) {
	bool repaint = false;
	if (d->m_type != ButtonState::Normal)
		repaint = true;
	d->m_type = ButtonState::Normal;
	QMouseEvent *mouseEvent = static_cast<QMouseEvent *> (event);
	d->m_mousePoint = mouseEvent->pos();

	// 还原鼠标样式
	QApplication::restoreOverrideCursor();

	for (int i = 0; i < d->m_pushButtons.size(); ++i) {
		QStyleOptionButton button;
		int x = (option.rect.width() + option.rect.x()) - (d->m_pushButtons.size() - i) * (d->m_width + d->m_spacing);
		int y = option.rect.y() + (option.rect.height() - d->m_height) / 2;
		button.rect = QRect(x, y, d->m_width, d->m_height);

		// 鼠标不位于按钮之上，直接跳过循环
		if (!button.rect.contains(d->m_mousePoint))
			continue;

		repaint = true;
		switch (event->type())
		{
			// 鼠标滑过
		case QEvent::MouseMove:
		{
			// 设置鼠标样式为手型
			QApplication::setOverrideCursor(Qt::PointingHandCursor);

			d->m_type = ButtonState::Hover;
			showToolTip(Mc::exists(d->m_pushButtonTexts, i) ? d->m_pushButtonTexts.at(i) : "");
			break;
		}
		// 鼠标按下
		case QEvent::MouseButtonPress:
		{
			d->m_type = ButtonState::Press;
			break;
		}
		// 鼠标释放
		case QEvent::MouseButtonRelease:
		{
			Mc::exists(d->m_buttonFuncs, i) ? d->m_buttonFuncs.at(i)(mouseEvent->pos(), index) : false;
			break;
		}
		default:
			break;
		}
	}

	return repaint;
}

inline void McPlaylistDelegate::showToolTip(const QString &text) const {
	if (text.isEmpty())
		return;

	QToolTip::showText(QCursor::pos(), text);
}
