#include "McTcpClient.h"
#include <qtcpsocket.h>
#include "McOutput.h"

struct McTcpClientData {
	QTcpSocket *m_tcpClient;
	QString m_ip;
	quint16 m_port;
	QString m_welcome;
};

McTcpClient::McTcpClient(const QString &ip, const quint16 &port, QObject *parent)
	: QObject(parent)
	, d(new McTcpClientData)
{
	d->m_ip = ip;
	d->m_port = port;

	connect(McOutputInstance, SIGNAL(message(QString)), this, SLOT(writeData(QString)));
}

McTcpClient::~McTcpClient(){
	if (d->m_tcpClient) {
		writeData("MyAudioPlayer Disconnected");
		d->m_tcpClient->disconnectFromHost();
	}
}

bool McTcpClient::init(const QString &welcome) {
	d->m_welcome = welcome;
	//初始化TCP客户端
	d->m_tcpClient = new QTcpSocket(this);   //实例化tcpClient
	d->m_tcpClient->abort();                 //取消原有连接
	d->m_tcpClient->connectToHost(d->m_ip, d->m_port);
	if (!d->m_tcpClient->waitForConnected(1000)) {  // 连接成功则进入if{}
		deleteTcpSocket();
		return false;
	}
	writeData(d->m_welcome);
	connect(d->m_tcpClient, SIGNAL(readyRead()), this, SLOT(readData()));
	connect(d->m_tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), \
		this, SLOT(readError(QAbstractSocket::SocketError)));
	return true;
}

void McTcpClient::readData() {
	QByteArray buffer = d->m_tcpClient->readAll();

}

void McTcpClient::readError(QAbstractSocket::SocketError) {
	d->m_tcpClient->disconnectFromHost();
	//QMessageBox::warning(NULL, "warning", tr("failed to connect server because %1").arg(d->m_tcpClient->errorString()));
	deleteTcpSocket();
}

void McTcpClient::writeData(const QString &msg) {
	if (!d->m_tcpClient) {
		if (!init(d->m_welcome))
			return;
	}
	d->m_tcpClient->write(msg.toUtf8().toBase64()); //qt5除去了.toAscii()
	d->m_tcpClient->flush();
}

void McTcpClient::deleteTcpSocket() {
	d->m_tcpClient->disconnect();
	d->m_tcpClient->deleteLater();
	d->m_tcpClient = NULL;
}
