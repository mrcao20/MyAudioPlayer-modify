#include "McNetTools.h"
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qjsonvalue.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qeventloop.h>
#include <qregularexpression.h>
#include "McGlobal.h"

struct McNetToolsData {
	QNetworkAccessManager m_networkAccessManager;
};

McNetTools::McNetTools(QObject *parent)
	: QObject(parent)
	, d(new McNetToolsData)
{

}

bool McNetTools::isValid(const QString &url) {
	QNetworkRequest request;
	request.setUrl(url);
	QNetworkReply *reply = d->m_networkAccessManager.get(request);
	bool valid = reply->waitForReadyRead(500);
	reply->abort();
	reply->deleteLater();
	return valid;
}

/*
����ָ����nodeName��QByteArray���ҳ���һ������������QJsonValue
*/
QJsonValue McNetTools::getJsonValue(const QByteArray &json, const QString &nodeName) {
	return getJsonValue(json, nodeName, 1);
}

QJsonValue McNetTools::getJsonValue(const QByteArray &json, const QString &nodeName, int subscript) {
	QJsonValue nodeValue;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(json);
	if (jsonDocument.isEmpty()) {
		mcOutput << "jsonֵ��Ч";
		return nodeValue;
	}
	QJsonObject root = jsonDocument.object();
	nodeValue = getJsonValue(root, nodeName, subscript);
	return nodeValue;
}

/*
����nodeName��QJsonObject���ҳ���һ������������QJsonValue
*/
QJsonValue McNetTools::getJsonValue(const QJsonObject &root, const QString &nodeName) {
	return getJsonValue(root, nodeName, 1);
}

QJsonValue McNetTools::getJsonValue(const QJsonObject &root, const QString &nodeName, int subscript) {
	QJsonValue nodeValue;
	QStringList nodeList = nodeName.split(QRegularExpression("\\."), QString::SkipEmptyParts);
	QJsonObject jsonObject = root;
	getValue(jsonObject, nodeValue, nodeList, subscript);
	return nodeValue;
}

/*
	ÿ�ν���ú���������һ��nodeList��ÿ��ѭ�������һ��node���ң��������������������
	�����������ݸ�node��Ӧ����������ң�������ҵ����һ���ڵ㣬������Ҫ���ҵ��±꣬
	����ͬ�������һ���ڵ����Ӧ�±�λ�õĶ���,������±�λ�ö��󲻴��ڣ������η���ǰһ��
*/
void McNetTools::getValue(QJsonObject &root, QJsonValue &nodeValue, QStringList &nodeList, int subscript) {
	QStringList list = nodeList;
	int curSubscript = 1;
	for (int i = 0; i < nodeList.size(); i++) {
		nodeValue = root.value(list.takeFirst());   // ��������ڸ�node����nodeValue��typeΪUndefined
		if (nodeValue.type() != QJsonValue::Object && nodeValue.type() != QJsonValue::Array)
			break;
		if (i == nodeList.size() - 1)
			break;
		if (nodeValue.type() == QJsonValue::Object) {
			root = nodeValue.toObject();
			continue;
		}
		QJsonArray array = nodeValue.toArray();
		for (auto itr = array.begin(); itr != array.end(); itr++) {
			root = (*itr).toObject();
			getValue(root, nodeValue, list, subscript);
			if (!nodeValue.isUndefined() && !nodeValue.isNull()) {
				if (curSubscript == subscript || list.size() > 1) {
					return;
				}
				else {
					curSubscript++;
					continue;
				}
			}
		}
	}
}

QByteArray McNetTools::getNetworkData(const QString &url, const QMap<QByteArray, QByteArray> &headers) {
	return getNetworkData(QUrl(url), headers);
}

QByteArray McNetTools::getNetworkData(const QUrl &url, const QMap<QByteArray, QByteArray> &headers) {
	QNetworkReply *reply = getNetworkReply(url, headers);
	QByteArray data = getNetworkData(reply);
	return data;
}

QByteArray McNetTools::getNetworkData(const QString &url, const QByteArray &postData, const QMap<QByteArray, QByteArray> &headers) {
	return getNetworkData(QUrl(url), postData, headers);
}

QByteArray McNetTools::getNetworkData(const QUrl &url, const QByteArray &postData, const QMap<QByteArray, QByteArray> &headers) {
	QNetworkReply *reply = getNetworkReply(url, postData, headers);
	QByteArray data = getNetworkData(reply);
	return data;
}

QNetworkReply *McNetTools::getNetworkReply(const QString &url, const QMap<QByteArray, QByteArray> &headers) {
	return getNetworkReply(QUrl(url), headers);
}

QNetworkReply *McNetTools::getNetworkReply(const QUrl &url, const QMap<QByteArray, QByteArray> &headers) {
	QNetworkRequest request;
	request.setUrl(url);
	for (auto itr = headers.cbegin(); itr != headers.cend(); itr++) {
		request.setRawHeader(itr.key(), itr.value());
	}
	QNetworkReply *reply = d->m_networkAccessManager.get(request);
	return reply;
}

QNetworkReply *McNetTools::getNetworkReply(const QString &url, const QByteArray &postData, const QMap<QByteArray, QByteArray> &headers) {
	return getNetworkReply(QUrl(url), postData, headers);
}

QNetworkReply *McNetTools::getNetworkReply(const QUrl &url, const QByteArray &postData, const QMap<QByteArray, QByteArray> &headers) {
	QNetworkRequest request;
	request.setUrl(url);
	for (auto itr = headers.cbegin(); itr != headers.cend(); itr++) {
		request.setRawHeader(itr.key(), itr.value());
	}
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	request.setHeader(QNetworkRequest::ContentLengthHeader, postData.length());
	QNetworkReply *reply = d->m_networkAccessManager.post(request, postData);
	return reply;
}

QByteArray McNetTools::getNetworkData(QNetworkReply *reply) {
	QEventLoop loop;
	connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec();
	QByteArray data = reply->readAll();
	reply->close();
	delete reply;
	return data;
}

McNetTools::~McNetTools(){
}
