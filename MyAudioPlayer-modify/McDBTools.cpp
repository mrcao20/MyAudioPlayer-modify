#include "McDBTools.h"
#include <qt_windows.h>
#include <qsqldatabase.h>
#include <qfile.h>
#include "McGlobal.h"

struct McDBToolsData {
	QString m_dbConnectionName;
	QSqlDatabase m_db;
	QSqlQuery m_query;
	bool m_hasNext;
	int m_fieldNum;
};

McDBTools::McDBTools(const QString &connectionName, QObject *parent)
	: QObject(parent)
	, d(new McDBToolsData)
{
	d->m_dbConnectionName = connectionName;
	d->m_hasNext = false;
	d->m_fieldNum = -1;
}

McDBTools::~McDBTools(){
	QString connectionName = d->m_dbConnectionName;
	/*	���ڴ˴�QSqlDatabase��QSqlQueryΪd�е����ԣ�����Ҫd����ʱ�Ż���������d����Ҫ����������ִ����֮��Ż�������
		����QSqlDatabase��removeDatabase������Ҫ��֮ǰ�������ݿ���Դ�ͷ����֮����ܳɹ����ʴ˴��ֶ�����d��Ȼ����
		�Ͽ����ӣ�ȷ�����ݿ���Դȫ���ͷ�
	*/
	d.reset();
	/*	����db��query��Ϊջ�����޷�ʹ��delete��������Ҳ����ֱ�ӵ��������������ͷ�����������ռ�õ���Դ��
		����ʱ����������δ���������ǻ��ڱ���������ʱ�Զ����������ڲ���Դ�Ѿ��ͷţ��ʿ��Ե���removeDatabase������
		�������Ǿ�������d�У��˴��Բ�������d�ķ�ʽ
	*/
	/*d->m_db.~QSqlDatabase();
	d->m_query.~QSqlQuery();*/
	// ����һ����d�Ѿ�����������������ֱ��ʹ��ԭʼ�ַ���
	QSqlDatabase::removeDatabase(connectionName);
}

bool McDBTools::init() {
	// ��⵱ǰ�����Ƿ����
	if (isDBExists())
		return true;
	bool isStartMySQL = startSvc("MYSQL57");
	QSqlDatabase db;
	if (isStartMySQL) {
		// ���MySQL57�����ܹ���������ʹ��mysql���ݿ�
		db = QSqlDatabase::addDatabase("QMYSQL", d->m_dbConnectionName);
		db.setHostName("localhost");
		db.setPort(3306);
		db.setDatabaseName("d_music_modify");
		db.setUserName("root");
		db.setPassword("839566521");
		mcOutput << "MySQL";
	}
	else {
		// ������ܿ�������ʹ��SQLite���ݿ�
		QString dbName = Mc::currAppPath() + "/data/SQLite/d_music_modify_data.db";
		bool isExist = QFile::exists(dbName);
		if (!isExist) {	// ������ݿ�������ļ������ڣ�����Ҫ�ӽṹ�ļ�����
			QString tempDBName = Mc::currAppPath() + "/data/SQLite/d_music_modify.db";
			if (!QFile::exists(tempDBName)) {	// ����ṹ�ļ�Ҳ�����ڣ����˳�����
				Mc::showWarning("���ݿ��ʧ�ܣ������˳�");
				return false;
			}
			QFile::copy(tempDBName, dbName);
		}
		db = QSqlDatabase::addDatabase("QSQLITE", d->m_dbConnectionName);
		db.setDatabaseName(dbName);
		db.setUserName("root");
		db.setPassword("839566521");
		mcOutput << "SQLite";
	}
	// �ȴ��������ݿ�
	while (!db.open()) {
		Sleep(300);
	}
	return true;
}

bool McDBTools::next() {
	d->m_hasNext = d->m_query.next();
	return (d->m_hasNext && d->m_fieldNum != -1);
}

QVector<QString> McDBTools::record() {
	QVector<QString> data;
	if (!d->m_hasNext)
		return data;
	data.reserve(d->m_fieldNum);
	readData(data, d->m_fieldNum);
	return data;
}

QVector<QVector<QString>> McDBTools::remainRecord() {
	QVector<QVector<QString>> datas;
	if (!next())
		return datas;
	readData(datas, d->m_fieldNum);
	return datas;
}

QVector<QVector<QString>> McDBTools::allRecord(){
	QVector<QVector<QString>> datas;
	if (d->m_query.isForwardOnly() || !d->m_query.first())
		return datas;
	readData(datas, d->m_fieldNum);
	return datas;
}

bool McDBTools::setAutoIncrement(const QString &table, int value) {
	if (table.isEmpty())
		return false;
	if (!startQuery())
		return false;
	QString sql;
	if (d->m_db.driverName() == "QMYSQL")
		sql = QString("ALTER TABLE %1 AUTO_INCREMENT = %2").arg(table).arg(value);
	else if (d->m_db.driverName() == "QSQLITE")
		sql = QString("UPDATE sqlite_sequence SET seq = %1 WHERE name = '%2';").arg(value).arg(table);
	return d->m_query.exec(sql);
}

bool McDBTools::readFromDB(const QVector<QString> &selectFields, const QString &table) {
	// ����ֶλ����Ϊ�գ���ֱ�ӷ���
	if (selectFields.isEmpty() || table.isEmpty())
		return false;
	QString selectField = "";
	for (int i = 0; i < selectFields.size(); ++i) {
		if (i == selectFields.size() - 1) {
			selectField += selectFields.at(i);
			break;
		}
		selectField += (selectFields.at(i) + ", ");
	}
	return readFromDB(selectFields.size(), selectField, table);
}

bool McDBTools::readFromDB(const QVector<QString> &selectFields, const QString &table, const QString &field, 
	const QString &value, bool isLike) {
	// ����ֶλ������where�ֶ���Ϊ�գ���ֱ�ӷ���
	if (selectFields.isEmpty() || table.isEmpty() || field.isEmpty())
		return false;
	QString symbol = "=";
	if (isLike)
		symbol = "LIKE";
	QString fields = "";
	for (int i = 0; i < selectFields.size(); i++) {
		if (i == selectFields.size() - 1) {	// ���һ�����⴦��
			fields += selectFields.at(i);
			break;
		}
		fields += (selectFields.at(i) + ", ");
	}
	QString where = QString(R"(%1 %2 "%3")").arg(field, symbol, value);
	return readFromDB(selectFields.size(), fields, table, where);
}

bool McDBTools::deleteFromDB(const QString &table) {
	// �������Ϊ�գ�ֱ�ӷ���
	if (table.isEmpty())
		return false;
	if (!startQuery())
		return false;
	QString sql = QString("DELETE FROM %1").arg(table);
	return d->m_query.exec(sql);
}

bool McDBTools::deleteFromDB(const QString &table, const QString &field, const QString &value) {
	// ���where�ֶ��������Ϊ�գ���ֱ�ӷ���
	if (field.isEmpty() || table.isEmpty())
		return false;
	QString where = QString(R"(%1 = "%2")").arg(field, value);
	return deleteFromDB(table, where);
}

bool McDBTools::insertIntoDB(const QString &table, const QVector<QString> &fields, const QVector<QString> &values) {
	// ���where�ֶ��������Ϊ�գ�����where�ֶ����������ֶ�ֵ��������ȣ���ֱ�ӷ���
	if (fields.isEmpty() || table.isEmpty() || fields.size() != values.size())
		return false;
	QString field = "";
	QString value = "";
	for (int i = 0; i < fields.size(); i++) {
		if (i == fields.size() - 1) {	// ���һ���ֶ����⴦��
			field += fields.at(i);
			value += ("\"" + values.at(i) + "\"");
			break;
		}
		field += (fields.at(i) + ", ");
		value += ("\"" + values.at(i) + "\", ");
	}
	return insertIntoDB_private(table, field, value);
}

bool McDBTools::updateDB(const QString &table, const QVector<QString> &setFields, const QVector<QString> &setValues,
	const QVector<QString> &whereFields, const QVector<QString> &whereValues) {
	// ���������set�ֶ�����where�ֶ���Ϊ�գ�����set�ֶ������ֶ�ֵ�������ȡ�where�ֶ������ֶ�ֵ���ȣ���ֱ�ӷ���
	if (table.isEmpty() || setFields.isEmpty() || whereFields.isEmpty() || setFields.size() != setValues.size() || whereFields.size() != whereValues.size())
		return false;
	QString set = "";
	QString where = "";
	for (int i = 0; i < setFields.size(); ++i) {
		if (i == setFields.size() - 1) {
			set += QString(R"(%1 = "%2")").arg(setFields.at(i), setValues.at(i));
			break;
		}
		set += QString(R"(%1 = "%2", )").arg(setFields.at(i), setValues.at(i));
	}
	for (int i = 0; i < whereFields.size(); ++i) {
		if (i == whereFields.size() - 1) {
			where += QString(R"(%1 = "%2")").arg(whereFields.at(i), whereValues.at(i));
			break;
		}
		where += QString(R"(%1 = "%2" AND )").arg(whereFields.at(i), whereValues.at(i));
	}
	return updateDB(table, set, where);
}

bool McDBTools::readFromDB(int fieldNum, const QString &selectFields, const QString &table) {
	// ����ֶλ����Ϊ�գ���ֱ�ӷ���
	if (selectFields.isEmpty() || table.isEmpty())
		return false;
	if (!startQuery())
		return false;
	d->m_fieldNum = fieldNum;
	return d->m_query.exec(QString("SELECT %1 FROM %2;").arg(selectFields, table));
}

bool McDBTools::readFromDB(int fieldNum, const QString &selectFields, const QString &table, const QString &where) {
	// ����ֶλ������where�Ӿ�Ϊ�գ���ֱ�ӷ���
	if (selectFields.isEmpty() || table.isEmpty() || where.isEmpty())
		return false;
	if (!startQuery())
		return false;
	d->m_fieldNum = fieldNum;
	return d->m_query.exec(QString("SELECT %1 FROM %2 WHERE %3;").arg(selectFields, table, where));
}

bool McDBTools::deleteFromDB(const QString &table, const QString &where) {
	// ���where�Ӿ�����Ϊ�գ���ֱ�ӷ���
	if (where.isEmpty() || table.isEmpty())
		return false;
	// ����޷���ʼִ�������ֱ�ӷ���
	if (!startQuery())
		return false;
	QString sql = QString("DELETE FROM %1 WHERE %2;").arg(table, where);
	return d->m_query.exec(sql);
}

bool McDBTools::insertIntoDB_private(const QString &table, const QString &fields, const QString &values) {
	// ���where�ֶ��������Ϊ�գ���ֱ�ӷ���
	if (fields.isEmpty() || table.isEmpty())
		return false;
	if (!startQuery())
		return false;
	QString sql = QString(R"(INSERT INTO %1 (%2) VALUES(%3);)").arg(table, fields, values);
	return d->m_query.exec(sql);
}

bool McDBTools::updateDB(const QString &table, const QString &set, const QString &where) {
	// ���������set�ֶ�����where�ֶ���Ϊ�գ���ֱ�ӷ���
	if (table.isEmpty() || set.isEmpty() || where.isEmpty())
		return false;
	if (!startQuery())
		return false;
	return d->m_query.exec(QString(R"(UPDATE %1 SET %2 WHERE %3;)").arg(table, set, where));
}

void McDBTools::readData(QVector<QString> &data, int fieldNum) {
	// ������ص�record������indexΪi��ֵʱ�����ᱨ��ֻ�᷵����Ч��QVariant���ٵ���toStringʱ���ؿ��ַ���
	for (int i = 0; i < fieldNum; ++i)
		data.append(d->m_query.value(i).toString());
}

void McDBTools::readData(QVector<QVector<QString>> &datas, int fieldNum) {
	QVector<QString> data;
	data.reserve(fieldNum);
	do {
		readData(data, fieldNum);
		datas.append(data);
	} while (d->m_query.next());
}

bool McDBTools::startSvc(const char *szServiceName) {
	SC_HANDLE schService;
	schService = OpenServiceA(OpenSCManagerA(NULL, NULL, GENERIC_READ), szServiceName, SERVICE_ALL_ACCESS);
	if (!schService) {
		mcOutput << QString("������Ϊ%1�����ݿ�δ�ҵ�").arg(szServiceName);
		return false;
	}
	SERVICE_STATUS svcStatus = { 0 };
	QueryServiceStatus(schService, &svcStatus);
	if (svcStatus.dwCurrentState == SERVICE_RUNNING) {
		CloseServiceHandle(schService);
		return true;
	}
	bool ret = StartServiceA(schService, 0, NULL);
	CloseServiceHandle(schService);
	if (!ret) {
		mcOutput << "���ݿ������ʧ��: " << GetLastError();
		return ret;
	}
	return ret;
}

inline bool McDBTools::isDBExists() {
	return QSqlDatabase::contains(d->m_dbConnectionName);
}

bool McDBTools::startQuery() {
	// ���ж����ݿ��Ƿ���ڣ�������ڣ��򲻻���ִ�г�ʼ����䡣��������ڣ���ִ�г�ʼ����䣬��ʼ��ʧ���򷵻�
	if (!isDBExists() && !init()) {
		mcOutput << "���ݿ�����ʧ�ܣ��޷�ִ�и����";
		return false;
	}
	d->m_query.clear();
	d->m_db.close();
	d->m_hasNext = false;
	d->m_fieldNum = -1;
	d->m_db = QSqlDatabase::database(d->m_dbConnectionName);
	d->m_query = QSqlQuery(d->m_db);
	d->m_query.setForwardOnly(true);
	return true;
}
