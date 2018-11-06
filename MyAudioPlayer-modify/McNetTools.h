#pragma once

#include <QObject>
#include <qmap.h>
#include <qscopedpointer.h>

QT_BEGIN_NAMESPACE
class QNetworkReply;
QT_END_NAMESPACE

struct McNetToolsData;

class McNetTools : public QObject{
	Q_OBJECT

public:
	explicit McNetTools(QObject *parent = 0);
	~McNetTools();

	bool isValid(const QString &url);
	QByteArray getNetworkData(const QString &, const QMap<QByteArray, QByteArray> & = QMap<QByteArray, QByteArray>());
	QByteArray getNetworkData(const QUrl &, const QMap<QByteArray, QByteArray> & = QMap<QByteArray, QByteArray>());
	QByteArray getNetworkData(const QString &, const QByteArray &postData, const QMap<QByteArray, QByteArray> & = QMap<QByteArray, QByteArray>());
	QByteArray getNetworkData(const QUrl &, const QByteArray &postData, const QMap<QByteArray, QByteArray> & = QMap<QByteArray, QByteArray>());
	QNetworkReply *getNetworkReply(const QString &, const QMap<QByteArray, QByteArray> & = QMap<QByteArray, QByteArray>());
	QNetworkReply *getNetworkReply(const QUrl &, const QMap<QByteArray, QByteArray> & = QMap<QByteArray, QByteArray>());
	QNetworkReply *getNetworkReply(const QString &, const QByteArray &postData, const QMap<QByteArray, QByteArray> & = QMap<QByteArray, QByteArray>());
	QNetworkReply *getNetworkReply(const QUrl &, const QByteArray &postData, const QMap<QByteArray, QByteArray> & = QMap<QByteArray, QByteArray>());
	QJsonValue getJsonValue(const QByteArray &json, const QString &nodeName);
	QJsonValue getJsonValue(const QByteArray &json, const QString &nodeName, int subscript);
	QJsonValue getJsonValue(const QJsonObject &root, const QString &nodeName);
	QJsonValue getJsonValue(const QJsonObject &root, const QString &nodeName, int subscript);

private:
	//根据所指定的nodelist返回第一个满足条件的nodeValue
	void getValue(QJsonObject &root, QJsonValue &nodeValue, QStringList &nodeList, int subscript);
	QByteArray getNetworkData(QNetworkReply *reply);

private:
	QScopedPointer<McNetToolsData> d;

};
