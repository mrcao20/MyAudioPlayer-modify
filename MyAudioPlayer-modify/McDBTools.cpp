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
	/*	由于此处QSqlDatabase和QSqlQuery为d中的属性，它需要d析构时才会析构，而d又需要本析构函数执行完之后才会析构，
		调用QSqlDatabase的removeDatabase函数需要将之前所有数据库资源释放完成之后才能成功，故此处手动析构d，然后再
		断开连接，确保数据库资源全部释放
	*/
	d.reset();
	/*	由于db和query均为栈对象，无法使用delete析构，但也可以直接调用析构函数，释放这两个对象占用的资源，
		但此时这两个对象并未析构，它们会在本对象析构时自动析构，但内部资源已经释放，故可以调用removeDatabase函数，
		由于它们均存在于d中，此处仍采用析构d的方式
	*/
	/*d->m_db.~QSqlDatabase();
	d->m_query.~QSqlQuery();*/
	// 在上一步中d已经被析构，所以这里直接使用原始字符串
	QSqlDatabase::removeDatabase(connectionName);
}

bool McDBTools::init() {
	// 检测当前连接是否存在
	if (isDBExists())
		return true;
	bool isStartMySQL = startSvc("MYSQL57");
	QSqlDatabase db;
	if (isStartMySQL) {
		// 如果MySQL57服务能够开启，则使用mysql数据库
		db = QSqlDatabase::addDatabase("QMYSQL", d->m_dbConnectionName);
		db.setHostName("localhost");
		db.setPort(3306);
		db.setDatabaseName("d_music_modify");
		db.setUserName("root");
		db.setPassword("839566521");
		mcOutput << "MySQL";
	}
	else {
		// 如果不能开启，则使用SQLite数据库
		QString dbName = Mc::currAppPath() + "/data/SQLite/d_music_modify_data.db";
		bool isExist = QFile::exists(dbName);
		if (!isExist) {	// 如果数据库的数据文件不存在，则需要从结构文件复制
			QString tempDBName = Mc::currAppPath() + "/data/SQLite/d_music_modify.db";
			if (!QFile::exists(tempDBName)) {	// 如果结构文件也不存在，则退出程序
				Mc::showWarning("数据库打开失败，程序退出");
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
	// 等待开启数据库
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
	// 如果字段或表名为空，则直接返回
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
	// 如果字段或表名、where字段名为空，则直接返回
	if (selectFields.isEmpty() || table.isEmpty() || field.isEmpty())
		return false;
	QString symbol = "=";
	if (isLike)
		symbol = "LIKE";
	QString fields = "";
	for (int i = 0; i < selectFields.size(); i++) {
		if (i == selectFields.size() - 1) {	// 最后一项特殊处理
			fields += selectFields.at(i);
			break;
		}
		fields += (selectFields.at(i) + ", ");
	}
	QString where = QString(R"(%1 %2 "%3")").arg(field, symbol, value);
	return readFromDB(selectFields.size(), fields, table, where);
}

bool McDBTools::deleteFromDB(const QString &table) {
	// 如果表名为空，直接返回
	if (table.isEmpty())
		return false;
	if (!startQuery())
		return false;
	QString sql = QString("DELETE FROM %1").arg(table);
	return d->m_query.exec(sql);
}

bool McDBTools::deleteFromDB(const QString &table, const QString &field, const QString &value) {
	// 如果where字段名或表名为空，则直接返回
	if (field.isEmpty() || table.isEmpty())
		return false;
	QString where = QString(R"(%1 = "%2")").arg(field, value);
	return deleteFromDB(table, where);
}

bool McDBTools::insertIntoDB(const QString &table, const QVector<QString> &fields, const QVector<QString> &values) {
	// 如果where字段名或表名为空，或者where字段名数量和字段值数量不相等，则直接返回
	if (fields.isEmpty() || table.isEmpty() || fields.size() != values.size())
		return false;
	QString field = "";
	QString value = "";
	for (int i = 0; i < fields.size(); i++) {
		if (i == fields.size() - 1) {	// 最后一个字段特殊处理
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
	// 如果表名、set字段名或where字段名为空，或者set字段名和字段值数量不等、where字段名和字段值不等，则直接返回
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
	// 如果字段或表名为空，则直接返回
	if (selectFields.isEmpty() || table.isEmpty())
		return false;
	if (!startQuery())
		return false;
	d->m_fieldNum = fieldNum;
	return d->m_query.exec(QString("SELECT %1 FROM %2;").arg(selectFields, table));
}

bool McDBTools::readFromDB(int fieldNum, const QString &selectFields, const QString &table, const QString &where) {
	// 如果字段或表名、where子句为空，则直接返回
	if (selectFields.isEmpty() || table.isEmpty() || where.isEmpty())
		return false;
	if (!startQuery())
		return false;
	d->m_fieldNum = fieldNum;
	return d->m_query.exec(QString("SELECT %1 FROM %2 WHERE %3;").arg(selectFields, table, where));
}

bool McDBTools::deleteFromDB(const QString &table, const QString &where) {
	// 如果where子句或表名为空，则直接返回
	if (where.isEmpty() || table.isEmpty())
		return false;
	// 如果无法开始执行语句则直接返回
	if (!startQuery())
		return false;
	QString sql = QString("DELETE FROM %1 WHERE %2;").arg(table, where);
	return d->m_query.exec(sql);
}

bool McDBTools::insertIntoDB_private(const QString &table, const QString &fields, const QString &values) {
	// 如果where字段名或表名为空，则直接返回
	if (fields.isEmpty() || table.isEmpty())
		return false;
	if (!startQuery())
		return false;
	QString sql = QString(R"(INSERT INTO %1 (%2) VALUES(%3);)").arg(table, fields, values);
	return d->m_query.exec(sql);
}

bool McDBTools::updateDB(const QString &table, const QString &set, const QString &where) {
	// 如果表名、set字段名或where字段名为空，则直接返回
	if (table.isEmpty() || set.isEmpty() || where.isEmpty())
		return false;
	if (!startQuery())
		return false;
	return d->m_query.exec(QString(R"(UPDATE %1 SET %2 WHERE %3;)").arg(table, set, where));
}

void McDBTools::readData(QVector<QString> &data, int fieldNum) {
	// 如果返回的record不存在index为i的值时，不会报错，只会返回无效的QVariant，再调用toString时返回空字符串
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
		mcOutput << QString("服务名为%1的数据库未找到").arg(szServiceName);
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
		mcOutput << "数据库服务开启失败: " << GetLastError();
		return ret;
	}
	return ret;
}

inline bool McDBTools::isDBExists() {
	return QSqlDatabase::contains(d->m_dbConnectionName);
}

bool McDBTools::startQuery() {
	// 先判断数据库是否存在，如果存在，则不会再执行初始化语句。如果不存在，再执行初始化语句，初始化失败则返回
	if (!isDBExists() && !init()) {
		mcOutput << "数据库连接失败，无法执行该语句";
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
