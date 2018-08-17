#include "database.h"

DataBase::DataBase()
{

}

DataBase::~DataBase()
{

}

//建立一个数据库连接
bool DataBase::createConnection()
{
	//以后就可以用"sqlite1"与数据库进行连接了
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "sqlite1");
	db.setDatabaseName("HistoryScore.db");
	if (!db.open())
	{
		qDebug() << "cannot connect to the database";
		return false;
	}
	return true;
}

//创建数据库表
bool DataBase::createTable()
{
	QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
	QSqlQuery query(db);
	bool success = query.exec("create table historyscore(id int primary key,time varchar,score int)");
	if (success)
	{
		qDebug() << QObject::tr("database create successed!\n");
		return true;
	}
	else
	{
		QSqlError lastError = query.lastError();
		qDebug() << lastError.driverText() << QString(QObject::tr("database create failed!"));
		return false;
	}
}

//向数据库中插入记录
bool DataBase::insert(int id, QString time, int score)
{
	QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
	QSqlQuery query(db);
	query.prepare("insert into historyscore values(?,?,?)");

	query.bindValue(0, id);
	query.bindValue(1, time);
	query.bindValue(2, score);

	bool success = query.exec();
	if (!success)
	{
		QSqlError lastError = query.lastError();
		qDebug() << lastError.driverText() << QString(QObject::tr("insert fail!"));
		return false;
	}
	return true;
}

//查询前十个日期
QStringList DataBase::queryAllDate()
{
	QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
	QSqlQuery query(db);
	query.exec("select * from historyscore");
	QSqlRecord rec = query.record();
	qDebug() << QObject::tr("historyscore表字段数：") << rec.count();

	QStringList dateData;
	int row = 0;
	while (query.next())
	{
		dateData.append(query.value(1).toString());
		row++;
		if (row > 9)
			break;
	}
	return dateData;
}

//查询前十个分数
QList<int> DataBase::queryAllScore()
{
	QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
	QSqlQuery query(db);
	query.exec("select * from historyscore");
	QSqlRecord rec = query.record();
	qDebug() << QObject::tr("historyscore表字段数：") << rec.count();

	QList<int> scoreData;
	int row = 0;
	while (query.next())
	{
		scoreData.append(query.value(2).toInt());
		row++;
		if (row > 9)
			break;
	}
	return scoreData;
}
//根据ID删除记录
bool DataBase::deleteById(int id)
{
	QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
	QSqlQuery query(db);
	query.prepare(QString("delete from historyscore where id = %1").arg(id));
	if (!query.exec())
	{
		qDebug() << "delete failed!";
		return false;
	}
	return true;
}

//排序
bool DataBase::sortByScore()
{
	QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
	QSqlQuery query(db);
	bool success = query.exec("select * from historyscore order by score desc");
	if (success)
	{
		qDebug() << QObject::tr("sort succeed!");
		return true;
	}
	else
	{
		qDebug() << QObject::tr("sort failed！");
		return false;
	}
}