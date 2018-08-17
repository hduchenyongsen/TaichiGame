#include "database.h"

DataBase::DataBase()
{

}

DataBase::~DataBase()
{

}

//����һ�����ݿ�����
bool DataBase::createConnection()
{
	//�Ժ�Ϳ�����"sqlite1"�����ݿ����������
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "sqlite1");
	db.setDatabaseName("HistoryScore.db");
	if (!db.open())
	{
		qDebug() << "cannot connect to the database";
		return false;
	}
	return true;
}

//�������ݿ��
bool DataBase::createTable()
{
	QSqlDatabase db = QSqlDatabase::database("sqlite1"); //�������ݿ�����
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

//�����ݿ��в����¼
bool DataBase::insert(int id, QString time, int score)
{
	QSqlDatabase db = QSqlDatabase::database("sqlite1"); //�������ݿ�����
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

//��ѯǰʮ������
QStringList DataBase::queryAllDate()
{
	QSqlDatabase db = QSqlDatabase::database("sqlite1"); //�������ݿ�����
	QSqlQuery query(db);
	query.exec("select * from historyscore");
	QSqlRecord rec = query.record();
	qDebug() << QObject::tr("historyscore���ֶ�����") << rec.count();

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

//��ѯǰʮ������
QList<int> DataBase::queryAllScore()
{
	QSqlDatabase db = QSqlDatabase::database("sqlite1"); //�������ݿ�����
	QSqlQuery query(db);
	query.exec("select * from historyscore");
	QSqlRecord rec = query.record();
	qDebug() << QObject::tr("historyscore���ֶ�����") << rec.count();

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
//����IDɾ����¼
bool DataBase::deleteById(int id)
{
	QSqlDatabase db = QSqlDatabase::database("sqlite1"); //�������ݿ�����
	QSqlQuery query(db);
	query.prepare(QString("delete from historyscore where id = %1").arg(id));
	if (!query.exec())
	{
		qDebug() << "delete failed!";
		return false;
	}
	return true;
}

//����
bool DataBase::sortByScore()
{
	QSqlDatabase db = QSqlDatabase::database("sqlite1"); //�������ݿ�����
	QSqlQuery query(db);
	bool success = query.exec("select * from historyscore order by score desc");
	if (success)
	{
		qDebug() << QObject::tr("sort succeed!");
		return true;
	}
	else
	{
		qDebug() << QObject::tr("sort failed��");
		return false;
	}
}