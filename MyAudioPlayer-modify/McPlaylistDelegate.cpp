#include "McPlaylistDelegate.h"
#include <qpushbutton.h>
#include <qevent.h>
#include <qapplication.h>
#include <qtooltip.h>
#include <qpainter.h>
#include "McGlobal.h"

struct McPlaylistDelegateData {
	QPoint m_mousePoint;  // ���λ��
	QVector<QPushButton *> m_pushButtons;
	QVector<QString> m_pushButtonTexts;
	QVector<std::function<void(const QPoint &, const QModelIndex &)>> m_buttonFuncs;
	int m_spacing;  // ��ť֮��ļ��
	int m_width;  // ��ť���
	int m_height;  // ��ť�߶�
	McPlaylistDelegate::ButtonState m_type;  // ��ť״̬-1������ 2������
};

McPlaylistDelegate::McPlaylistDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
	, d(new McPlaylistDelegateData)
{
	d->m_pushButtons.append(new QPushButton());
	d->m_pushButtons.append(new QPushButton());
	d->m_pushButtons.append(new QPushButton());
	d->m_pushButtonTexts << "����" << "��λ"  << "����";
	d->m_buttonFuncs.append(std::bind(&McPlaylistDelegate::play, this, std::placeholders::_1, std::placeholders::_2));
	d->m_buttonFuncs.append(std::bind(&McPlaylistDelegate::toPosition, this, std::placeholders::_1, std::placeholders::_2));
	d->m_buttonFuncs.append(std::bind(&McPlaylistDelegate::more, this, std::placeholders::_1, std::placeholders::_2));
	d->m_spacing = 5;
	d->m_width = 25;
	d->m_height = 20;
	d->m_type = ButtonState::Normal;
	d->m_mousePoint = QPoint(-1, -1);

	// ���ð�ť������������������ʽ
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
		// ��������������state��ȥ��State_HasFocus״̬
		viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

	QStyledItemDelegate::paint(painter, viewOption, index);
	
	if (option.state.testFlag(QStyle::State_MouseOver) || option.state.testFlag(QStyle::State_Selected)) {
		for (int i = 0; i < d->m_pushButtons.size(); ++i) {
			// ���ư�ť
			QStyleOptionButton button;
			// ��ǰ�����ұߵĵ��ȥ��ť��ȼӼ������ʣ�ఴť������
			int x = (option.rect.width() + option.rect.x()) - (d->m_pushButtons.size() - i) * (d->m_width + d->m_spacing);
			// ��ǰ���yֵ���ϵ�ǰ��ĸ߶ȼ���ť��ȳ��Զ�
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

	// ��ԭ�����ʽ
	QApplication::restoreOverrideCursor();

	for (int i = 0; i < d->m_pushButtons.size(); ++i) {
		QStyleOptionButton button;
		int x = (option.rect.width() + option.rect.x()) - (d->m_pushButtons.size() - i) * (d->m_width + d->m_spacing);
		int y = option.rect.y() + (option.rect.height() - d->m_height) / 2;
		button.rect = QRect(x, y, d->m_width, d->m_height);

		// ��겻λ�ڰ�ť֮�ϣ�ֱ������ѭ��
		if (!button.rect.contains(d->m_mousePoint))
			continue;

		repaint = true;
		switch (event->type())
		{
			// ��껬��
		case QEvent::MouseMove:
		{
			// ���������ʽΪ����
			QApplication::setOverrideCursor(Qt::PointingHandCursor);

			d->m_type = ButtonState::Hover;
			showToolTip(Mc::exists(d->m_pushButtonTexts, i) ? d->m_pushButtonTexts.at(i) : "");
			break;
		}
		// ��갴��
		case QEvent::MouseButtonPress:
		{
			d->m_type = ButtonState::Press;
			break;
		}
		// ����ͷ�
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
