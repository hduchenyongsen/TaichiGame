#ifndef DATABASE_H
#define DATABASE_H
#include <QTextCodec>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QtDebug>
#include <QSqlDriver>
#include <QSqlRecord>

class DataBase
{
public:
	DataBase();
	~DataBase();
	bool createConnection();  //����һ������
	bool createTable();       //�������ݿ��
	bool insert(int id, QString time, int score);   //��������
	QStringList queryAllDate();          //��ѯǰʮ������
	QList<int> queryAllScore();			//��ѯǰʮ������
	bool updateById(int id);  //����
	bool deleteById(int id);  //ɾ��
	bool sortByScore();          //����

private:
	
};

#endif // DATABASE_H
