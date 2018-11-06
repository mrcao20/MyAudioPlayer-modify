#include "McAudioPlayer.h"
#include <qapplication.h>
#include <qsystemtrayicon.h>
#include <qmenu.h>
#include "McTcpClient.h"
#include "McAudioPlayerWidget.h"
#include "McGlobal.h"

struct McAudioPlayerData {
	McTcpClient *m_tcpClient;
	McAudioPlayerWidget *m_playerWidget;
};

McAudioPlayer::McAudioPlayer(QWidget *parent)
	: QWidget(parent)
	, d(new McAudioPlayerData)
{
	
}

McAudioPlayer::~McAudioPlayer() {

}

void McAudioPlayer::init() {
	d->m_tcpClient = new McTcpClient("127.0.0.1", 8000, this);
	d->m_tcpClient->init("McAudioPlayer Connected");
	initTrayMenu();

	d->m_playerWidget = new McAudioPlayerWidget(this);
	if (!d->m_playerWidget->init(this))
		exit(1);
}

void McAudioPlayer::quit() {
	QApplication::exit(0);
}

void McAudioPlayer::changeDisplay() {
	if (isVisible())
		hide();
	else
		show();
}

// 创建托盘
void McAudioPlayer::initTrayMenu() {
#ifdef DEBUG
	QString trayTip = "Audio Player Debug";
#else
	QString trayTip = "Audio Player";
#endif // DEBUG

	QSystemTrayIcon *systemTray = new QSystemTrayIcon(this);
	systemTray->setToolTip(trayTip);
	systemTray->setIcon(QIcon(":/McAudioPlayer/data/icon/AudioPlayer.png"));
	QMenu *trayMenu = new QMenu(this);
	QAction *showAction = trayMenu->addAction("Show");
	trayMenu->addSeparator();
	QAction *quitAction = trayMenu->addAction("Quit");
	systemTray->setContextMenu(trayMenu);
	systemTray->show();

	connect(showAction, &QAction::triggered, this, &QWidget::show);
	connect(quitAction, &QAction::triggered, this, &McAudioPlayer::quit);
	connect(systemTray, &QSystemTrayIcon::activated, [this](QSystemTrayIcon::ActivationReason reason) {
		switch (reason) {
			//双击托盘显示窗口
		case QSystemTrayIcon::DoubleClick: {
			changeDisplay();
			break;
		}
		default:
			break;
		}
	});
}

// 只由play调用
void McAudioPlayer::playFile(const QString &filePath){
	d->m_playerWidget->play(filePath);
}
