#include "LyricLabel.h"
#include <qtimer.h>
#include <QPainter>

LyricLabel::LyricLabel(QWidget *parent)
	: QLabel(parent) 
	, m_isPlayed(true){

	//��ʵ����Խ������
	m_linear_gradient.setStart(0, 10);//�����������
	m_linear_gradient.setFinalStop(0, 40);//�����յ�����
										//��һ�������յ����꣬��������������������ԣ����ձ������м���
	m_linear_gradient.setColorAt(0.1, QColor(14, 179, 255));
	m_linear_gradient.setColorAt(0.5, QColor(114, 232, 255));
	m_linear_gradient.setColorAt(0.9, QColor(14, 179, 255));

	// ���ֵ����Խ������
	m_mask_linear_gradient.setStart(0, 10);
	m_mask_linear_gradient.setFinalStop(0, 40);
	m_mask_linear_gradient.setColorAt(0.1, QColor(222, 54, 4));
	m_mask_linear_gradient.setColorAt(0.5, QColor(255, 72, 16));
	m_mask_linear_gradient.setColorAt(0.9, QColor(222, 54, 4));

	// ��������
	m_font.setFamily("Times New Roman");
	m_font.setBold(true);
	m_font.setPointSize(24);
	
	// ���ö�ʱ��
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
	m_timer->setInterval(30);
	m_lrc_mask_width = 0;
	m_lrc_mask_width_interval = 0;
	show();
}

void LyricLabel::setText(const QString &text) {
	m_isPlayed = false;
	m_lrc_mask_width = 0; 
	QLabel::setText(text);
	update();
}

void LyricLabel::reset() {
	clear();
	m_isPlayed = true;
	m_timer->stop();
	m_lrc_mask_width = 0;
	m_lrc_mask_width_interval = 0;
	update();
}

// ���뵱ǰ���ʣ��Ĳ���ʱ��
void LyricLabel::start_lrc_mask(qint64 intervaltime)
{
	qreal count = (m_intervaltime - intervaltime) / 30;
	m_lrc_mask_width = m_lrc_mask_width_interval * count;
	m_timer->start();
	update();
}

// �������θ�ʲ��������ʱ��
void LyricLabel::set_lrc_mask(qint64 intervaltime) {
	m_intervaltime = intervaltime;
	qreal count = intervaltime / 30;
	m_lrc_mask_width_interval = width() / count;
	m_lrc_mask_width = 0;
}

void LyricLabel::stop_lrc_mask()
{
	m_isPlayed = true;
	m_timer->stop();
	m_lrc_mask_width = width();
	update();
}

void LyricLabel::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setFont(m_font);

	// �Ȼ��Ƶײ����֣���Ϊ��Ӱ��������ʹ��ʾЧ�������������Ҹ����ʸ�
	painter.setPen(QColor(0, 0, 0, 200));
	painter.drawText(1, 1, width(), height(), Qt::AlignLeft | Qt::AlignVCenter, text());//�����

														   // ����������ƽ�������
	painter.setPen(QPen(m_linear_gradient, 0));
	painter.drawText(0, 0, width(), height(), Qt::AlignLeft | Qt::AlignVCenter, text());

	// ���ø������
	painter.setPen(QPen(m_mask_linear_gradient, 0));
	painter.drawText(0, 0, m_lrc_mask_width, height(), Qt::AlignLeft | Qt::AlignVCenter, text());

	m_showProgress = m_lrc_mask_width / m_lrc_mask_width_interval * 30;
}

void LyricLabel::timeout()
{
	if (m_lrc_mask_width > width()) {
		emit lyricPlayedOver(this);
	}
	//ÿ��һ�ι̶���ʱ�����ֵĳ��Ⱦ�����һ��
	m_lrc_mask_width += m_lrc_mask_width_interval;
	update();//����widget�����ǲ��������ػ棬���ǰ���һ��Paint�¼�����������ѭ��ʱ��ϵͳ���ػ�
}