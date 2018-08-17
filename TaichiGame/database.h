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
	bool createConnection();  //创建一个连接
	bool createTable();       //创建数据库表
	bool insert(int id, QString time, int score);   //出入数据
	QStringList queryAllDate();          //查询前十个日期
	QList<int> queryAllScore();			//查询前十个分数
	bool updateById(int id);  //更新
	bool deleteById(int id);  //删除
	bool sortByScore();          //排序

private:
	
};

#endif // DATABASE_H
