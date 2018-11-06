#include "McSongDelegate.h"
#include <qpainter.h>
#include <qtooltip.h>
#include <qpushbutton.h>
#include <qevent.h>
#include <qapplication.h>
#include <qtimer.h>
#include "McGlobal.h"

struct McSongDelegateData {
	QPoint m_mousePoint;  // ���λ��
	QVector<QPushButton *> m_pushButtons;
	QVector<QString> m_pushButtonTexts;
	QVector<std::function<void(const QPoint &, const QModelIndex &)>> m_buttonFuncs;
	int m_spacing;  // ��ť֮��ļ��
	int m_width;  // ��ť���
	int m_height;  // ��ť�߶�
	McSongDelegate::ButtonState m_type;  // ��ť״̬-1������ 2������
	QTimer *m_showItemTextTimer;
	QString m_itemText;
	QModelIndex m_currIndex;
};

McSongDelegate::McSongDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
	, d(new McSongDelegateData)
{
	d->m_pushButtons.append(new QPushButton());
	d->m_pushButtons.append(new QPushButton());
	d->m_pushButtons.append(new QPushButton());
	d->m_pushButtons.append(new QPushButton());
	d->m_pushButtonTexts << "����" << "��ӵ�" << "����" << "����";
	d->m_buttonFuncs.append(std::bind(&McSongDelegate::play, this, std::placeholders::_1, std::placeholders::_2));
	d->m_buttonFuncs.append(std::bind(&McSongDelegate::addTo, this, std::placeholders::_1, std::placeholders::_2));
	d->m_buttonFuncs.append(std::bind(&McSongDelegate::download, this, std::placeholders::_1, std::placeholders::_2));
	d->m_buttonFuncs.append(std::bind(&McSongDelegate::more, this, std::placeholders::_1, std::placeholders::_2));
	d->m_spacing = 5;
	d->m_width = 25;
	d->m_height = 20;
	d->m_type = ButtonState::Normal;
	d->m_mousePoint = QPoint(-1, -1);
	d->m_itemText = "";
	d->m_showItemTextTimer = new QTimer(this);
	d->m_showItemTextTimer->setInterval(500);

	connect(d->m_showItemTextTimer, &QTimer::timeout, [this]() {
		showToolTip(d->m_itemText);
		d->m_showItemTextTimer->stop();
	});

	// ���ð�ť������������������ʽ
	d->m_pushButtons.at(0)->setStyleSheet("QPushButton {border: none; background-color: transparent; image:url(:/McAudioPlayer/data/icon/playButton.png);} \
                                QPushButton:hover {image:url(:/McAudioPlayer/data/icon/playButton_hover.png);}	\
								QPushButton:pressed {image:url(:/McAudioPlayer/data/icon/playButton_hover.png);}");

	d->m_pushButtons.at(1)->setStyleSheet("QPushButton {border: none; background-color: transparent; image:url(:/McAudioPlayer/data/icon/addTo.png);} \
                                QPushButton:hover {image:url(:/McAudioPlayer/data/icon/addTo_hover.png);}	\
								QPushButton:pressed {image:url(:/McAudioPlayer/data/icon/addTo_hover.png);}");

	d->m_pushButtons.at(2)->setStyleSheet("QPushButton {border: none; background-color: transparent; image:url(:/McAudioPlayer/data/icon/download.png);} \
                                QPushButton:hover {image:url(:/McAudioPlayer/data/icon/download_hover.png);}	\
								QPushButton:pressed {image:url(:/McAudioPlayer/data/icon/download_hover.png);}");

	d->m_pushButtons.at(3)->setStyleSheet("QPushButton {border: none; background-color: transparent; image:url(:/McAudioPlayer/data/icon/more.png);} \
                                QPushButton:hover {image:url(:/McAudioPlayer/data/icon/more_hover.png);}	\
								QPushButton:pressed {image:url(:/McAudioPlayer/data/icon/more_hover.png);}");

}

McSongDelegate::~McSongDelegate() {
}

void McSongDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
	QStyleOptionViewItem viewOption(option);
	initStyleOption(&viewOption, index);

	if (option.state.testFlag(QStyle::State_HasFocus))
		// ��������������state��ȥ��State_HasFocus״̬
		viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

	QStyledItemDelegate::paint(painter, viewOption, index);

	if (index == d->m_currIndex) {
		d->m_itemText = "";
		if (d->m_type == ButtonState::Normal)
			QToolTip::hideText();
	}
	
	//�����껬���ı���ɫ
	if (option.state.testFlag(QStyle::State_MouseOver)) {
		// Model��������ɫ��ʹ��Ĭ�ϴ�����䱳����������Ҫ�ֶ�����
		//painter->fillRect(view_option.rect, QColor(180, 200, 220));
		if (d->m_type == ButtonState::Normal) {
			d->m_itemText = index.data(Qt::DisplayRole).toString();
			d->m_currIndex = index;
		}
	}
	
	if (option.state.testFlag(QStyle::State_MouseOver) || option.state.testFlag(QStyle::State_Selected)) {
		if (index.column() == 0) {
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
				/*QString text = index.data(Qt::DisplayRole).toString();
				int align = index.data(Qt::TextAlignmentRole).toInt();
				painter->drawText(option.rect, align, "...");*/
			}
		}
	}
}

bool McSongDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) {
	d->m_showItemTextTimer->start();
	if (index.column() != 0)
		return false;
	bool repaint = false;
	if (d->m_type != ButtonState::Normal)
		repaint = true;
	d->m_type = ButtonState::Normal;
	QMouseEvent *mouseEvent = static_cast<QMouseEvent *> (event);
	d->m_mousePoint = mouseEvent->pos();

	// ��ԭ�����ʽ
	QApplication::restoreOverrideCursor();

	for (int i = 0; i < d->m_pushButtons.size(); ++i){
		QStyleOptionButton button;
		int x = (option.rect.width() + option.rect.x()) - (d->m_pushButtons.size() - i) * (d->m_width + d->m_spacing);
		int y = option.rect.y() + (option.rect.height() - d->m_height) / 2;
		button.rect = QRect(x, y, d->m_width, d->m_height);

		// ��겻λ�ڰ�ť֮�ϣ�ֱ������ѭ��
		if (!button.rect.contains(d->m_mousePoint))
			continue;

		repaint = true;
		d->m_showItemTextTimer->stop();
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

inline void McSongDelegate::showToolTip(const QString &text) const {
	if (text.isEmpty())
		return;

	QToolTip::showText(QCursor::pos(), text);
}
