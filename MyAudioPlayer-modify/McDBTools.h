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

	bool next();	// ָ����һ������
	QVector<QString> record();	// ��ȡ��ǰ����
	QVector<QVector<QString>> remainRecord();	// ��ȡ���һ�ε���next֮������ݣ���������nextʱ���൱�ڷ�����������
	QVector<QVector<QString>> allRecord();	// ��ȡ�������ݣ�����Ҫ����next������

	// ���ñ�����������ʼֵ
	bool setAutoIncrement(const QString &table, int value);

	// �����ѯ�ֶΣ�������where�ֶ������ֶ�ֵ
	bool readFromDB(const QString &selectField, const QString &table) {
		return readFromDB(QVector<QString>() << selectField, table);
	}
	bool readFromDB(const QVector<QString> &selectFields, const QString &table);
	bool readFromDB(const QString &selectField, const QString &table, const QString &field, const QString &value, bool isLike = false) {
		return readFromDB(QVector<QString>() << selectField, table, field, value, isLike);
	}
	bool readFromDB(const QVector<QString> &selectFields, const QString &table, const QString &field, const QString &value, bool isLike = false);

	// ��ձ�
	bool deleteFromDB(const QString &table);
	// ����������ֶ������ֶ�ֵ
	bool deleteFromDB(const QString &table, const QString &field, const QString &value);

	// ����������ֶ������ֶ�ֵ
	bool insertIntoDB(const QString &table, const QString &field, const QString &value) {
		return insertIntoDB(table, QVector<QString>() << field, QVector<QString>() << value);
	}
	bool insertIntoDB(const QString &table, const QVector<QString> &fields, const QVector<QString> &values);

	// ���������set���ֶ������ֶ�ֵ��where���ֶ������ֶ�ֵ
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
	// �����ݿ��ж�ȡ����
	bool readFromDB(int fieldNum, const QString &selectFields, const QString &table);
	/*	selectFields��ʽΪfield1, filed2����*�����Ҳ���Ϊ�գ�table��ʽΪtable1, table2�����Ҳ���Ϊ��
		where�Ӿ��ʽΪ��field1 = "value1" AND field2 = "value2"
	*/
	bool readFromDB(int fieldNum, const QString &selectFields, const QString &table, const QString &where);
	// ɾ�����ݿ��е�����
	bool deleteFromDB(const QString &table, const QString &where);
	// fields��ʽΪfield1, fiedl2��values��ʽΪ"value1", "value2"
	bool insertIntoDB_private(const QString &table, const QString &fields, const QString &values);
	// set��ʽΪfield1 = "value1", field2 = "value2"��where��ʽΪfield1 = "value1" AND field2 = "value2"
	bool updateDB(const QString &table, const QString &set, const QString &where);
	void readData(QVector<QString> &data, int fieldNum);
	void readData(QVector<QVector<QString>> &datas, int fieldNum);
	bool startSvc(const char *szServiceName);
	inline bool isDBExists();	// ������ݿ��Ƿ����
	bool startQuery();	// ����Ƿ���Կ�ʼִ�����

private:
	QScopedPointer<McDBToolsData> d;
};
