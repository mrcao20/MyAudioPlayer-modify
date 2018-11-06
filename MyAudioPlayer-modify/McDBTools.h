#pragma once

#include <QObject>
#include <qscopedpointer.h>
#include <qvector.h>
#include <qsqlquery.h>

struct McDBToolsData;

class McDBTools : public QObject {
	Q_OBJECT

public:
	explicit McDBTools(const QString &connectionName, QObject *parent = 0);
	~McDBTools();

	bool init();

	bool next();	// 指向下一个数据
	QVector<QString> record();	// 获取当前数据
	QVector<QVector<QString>> remainRecord();	// 获取最后一次调用next之后的数据，当不调用next时，相当于返回所有数据
	QVector<QVector<QString>> allRecord();	// 获取所有数据，不需要调用next即返回

	// 设置表的自增主码初始值
	bool setAutoIncrement(const QString &table, int value);

	// 传入查询字段，表名，where字段名和字段值
	bool readFromDB(const QString &selectField, const QString &table) {
		return readFromDB(QVector<QString>() << selectField, table);
	}
	bool readFromDB(const QVector<QString> &selectFields, const QString &table);
	bool readFromDB(const QString &selectField, const QString &table, const QString &field, const QString &value, bool isLike = false) {
		return readFromDB(QVector<QString>() << selectField, table, field, value, isLike);
	}
	bool readFromDB(const QVector<QString> &selectFields, const QString &table, const QString &field, const QString &value, bool isLike = false);

	// 清空表
	bool deleteFromDB(const QString &table);
	// 传入表名，字段名和字段值
	bool deleteFromDB(const QString &table, const QString &field, const QString &value);

	// 传入表名，字段名和字段值
	bool insertIntoDB(const QString &table, const QString &field, const QString &value) {
		return insertIntoDB(table, QVector<QString>() << field, QVector<QString>() << value);
	}
	bool insertIntoDB(const QString &table, const QVector<QString> &fields, const QVector<QString> &values);

	// 传入表名，set的字段名和字段值，where的字段名和字段值
	inline bool updateDB(const QString &table, const QString &setField, const QString &setValue,
		const QString &whereField, const QString &whereValue) {

		return updateDB(table, QVector<QString>() << setField, QVector<QString>() << setValue,
			QVector<QString>() << whereField, QVector<QString>() << whereValue);
	}
	inline bool updateDB(const QString &table, const QVector<QString> &setFields, const QVector<QString> &setValues,
		const QString &whereField, const QString &whereValue) {

		return updateDB(table, setFields, setValues,
			QVector<QString>() << whereField, QVector<QString>() << whereValue);
	}
	inline bool updateDB(const QString &table, const QString &setField, const QString &setValue,
		const QVector<QString> &whereFields, const QVector<QString> &whereValues) {

		return updateDB(table, QVector<QString>() << setField, QVector<QString>() << setValue,
			whereFields, whereValues);
	}
	bool updateDB(const QString &table, const QVector<QString> &setFields, const QVector<QString> &setValues,
					const QVector<QString> &whereFields, const QVector<QString> &whereValues);

private:
	// 从数据库中读取数据
	bool readFromDB(int fieldNum, const QString &selectFields, const QString &table);
	/*	selectFields格式为field1, filed2，或*，并且不能为空；table格式为table1, table2，并且不能为空
		where子句格式为，field1 = "value1" AND field2 = "value2"
	*/
	bool readFromDB(int fieldNum, const QString &selectFields, const QString &table, const QString &where);
	// 删除数据库中的数据
	bool deleteFromDB(const QString &table, const QString &where);
	// fields格式为field1, fiedl2，values格式为"value1", "value2"
	bool insertIntoDB_private(const QString &table, const QString &fields, const QString &values);
	// set格式为field1 = "value1", field2 = "value2"，where格式为field1 = "value1" AND field2 = "value2"
	bool updateDB(const QString &table, const QString &set, const QString &where);
	void readData(QVector<QString> &data, int fieldNum);
	void readData(QVector<QVector<QString>> &datas, int fieldNum);
	bool startSvc(const char *szServiceName);
	inline bool isDBExists();	// 检测数据库是否存在
	bool startQuery();	// 检测是否可以开始执行语句

private:
	QScopedPointer<McDBToolsData> d;
};
