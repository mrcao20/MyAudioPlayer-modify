#pragma once
#pragma execution_character_set("utf-8")
#include <QObject>
#include <qscopedpointer.h>
#include <qabstractsocket.h>

struct McTcpClientData;

class McTcpClient : public QObject {
	Q_OBJECT

public:
	explicit McTcpClient(const QString &ip, const quint16 &port, QObject *parent = 0);
	~McTcpClient();

	bool init(const QString &welcome);

private slots:
	//¿Í»§¶Ë²Ûº¯Êý
	void readData();
	void readError(QAbstractSocket::SocketError);
	void writeData(const QString &msg);

private:
	void deleteTcpSocket();

private:
	QScopedPointer<McTcpClientData> d;
};
