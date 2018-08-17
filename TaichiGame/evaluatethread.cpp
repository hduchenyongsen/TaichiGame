#include "evaluatethread.h"
#include <QtMath>
#include <QDebug>
#include <QFile>
#include <time.h>
#include <QMetaType>  

#define DYW_R 6
#define CHARACT_NUM 11

static QList<QList<qreal>> taichiDataQreal;

EvaluateThread::EvaluateThread(QObject *parent)
	: QThread(parent)
	, index(-1)
	, matchPositon(NonePositon)
{
	readTxt(":/TaichiGame/Resources/standardData.txt", taichiDataQreal);
	readTxt(":/TaichiGame/Resources/weight.txt", weightList);
	qRegisterMetaType<MatchPosition>("MatchPosition"); 
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(OnOver()));
}

EvaluateThread::~EvaluateThread()
{

}

void EvaluateThread::run()
{
	score();
	return;
}
//给出三个空间坐标，第二个为顶点，计算空间角度
qreal EvaluateThread::calculateAngle(Point3D pointA, Point3D pointB_Peak, Point3D pointC)
{
	QList<int> vectorBA = { pointA.x - pointB_Peak.x, pointA.y - pointB_Peak.y, pointA.z - pointB_Peak.z };
	QList<int> vectorBC = { pointC.x - pointB_Peak.x, pointC.y - pointB_Peak.y, pointC.z - pointB_Peak.z };
	qreal cosValue = (vectorBA[0] * vectorBC[0] + vectorBA[1] * vectorBC[1] + vectorBA[2] * vectorBC[2])
		/ (qSqrt(qPow(vectorBA[0], 2) + qPow(vectorBA[1], 2) + qPow(vectorBA[2], 2))
		* qSqrt(qPow(vectorBC[0], 2) + qPow(vectorBC[1], 2) + qPow(vectorBC[2], 2)));
	if (cosValue < -1.0)
		cosValue = -1.0;
	if (cosValue > 1.0)
		cosValue = 1.0;
	qreal degree = qRadiansToDegrees(qAcos(cosValue));
	return qRadiansToDegrees(qAcos(cosValue));
} 
//计算用于dtw的角度序列，传入的是9列的二维数组，9列---构成一个角度的三个关节点数据
QList<qreal> EvaluateThread::caculateSerialsAngle(QList<QList<int>> data)
{
	QList<qreal> angles;
	Point3D pointA;
	Point3D pointB_Peak;
	Point3D pointC;
	for (int i = 0; i < data.count(); i++)
	{
		pointA.x = data[i][0];
		pointA.y = data[i][1];
		pointA.z = data[i][2];
		pointB_Peak.x = data[i][3];
		pointB_Peak.y = data[i][4];
		pointB_Peak.z = data[i][5];
		pointC.x = data[i][6];
		pointC.y = data[i][7];
		pointC.z = data[i][8];
		angles.append(calculateAngle(pointA, pointB_Peak, pointC));
	}
	return angles;
}
//获取9列数据，按序传入3个点的x轴数据的列数，注意pointB是peak点
QList<QList<int>> EvaluateThread::get9ColumnData(QList<QList<int>> allData, 
	int pointABegin, int pointBBegin, int pointCBegin)
{
	QList<QList<int>> result;
	for (int i = 0; i < allData.count(); i++)
	{
		QList<int> temp;
		temp.append(allData[i].mid(pointABegin, 3));
		temp.append(allData[i].mid(pointBBegin, 3));
		temp.append(allData[i].mid(pointCBegin, 3));
		result.append(temp);
	}
	return result;
}
//从主线程获取数据，并按需要截取数个9列的数据
void EvaluateThread::OnGetData(int _index, QList<QList<int>> data)
{
	if (_index != 29)
		timer->start(12000);
	else
		timer->stop();

	for (int i = 0; i < data.count(); i++)
	{
		if (data[i].count() != 75)
		{
			qDebug() << data[i].count() << "dataget";
			return;
		}

	}
	 
	index = _index;

	//以下运算大约耗时3-11毫秒

	leftElbow = get9ColumnData(data, 3 * JointType::ShoulderLeft,
		3 * JointType::ElbowLeft, 3 * JointType::WristLeft);

	rightElbow = get9ColumnData(data, 3 * JointType::ShoulderRight,
		3 * JointType::ElbowRight, 3 * JointType::WristRight);

	leftArmToShoulder = get9ColumnData(data, 3 * JointType::ElbowLeft,
		3 * JointType::ShoulderLeft, 3 * JointType::SpineShoulder);

	rightArmToShoulder = get9ColumnData(data, 3 * JointType::ElbowRight,
		3 * JointType::ShoulderRight, 3 * JointType::SpineShoulder);

	leftArmToVertical = get9ColumnData(data, 3 * JointType::ElbowLeft,
		3 * JointType::ShoulderLeft, 3 * JointType::HipLeft);

	rightArmToVertical = get9ColumnData(data, 3 * JointType::ElbowRight,
		3 * JointType::ShoulderRight, 3 * JointType::HipRight);

	leftKnee = get9ColumnData(data, 3 * JointType::HipLeft,
		3 * JointType::KneeLeft, 3 * JointType::AnkleLeft);

	rightKnee = get9ColumnData(data, 3 * JointType::HipRight,
		3 * JointType::KneeRight, 3 * JointType::AnkleRight);

	hipLeft = get9ColumnData(data, 3 * JointType::ShoulderLeft,
		3 * JointType::HipLeft, 3 * JointType::KneeLeft);

	hipRight = get9ColumnData(data, 3 * JointType::ShoulderRight,
		3 * JointType::HipRight, 3 * JointType::KneeRight);

	crotch = get9ColumnData(data, 3 * JointType::KneeLeft,
		3 * JointType::SpineBase, 3 * JointType::KneeRight);

	this->start();
}
void EvaluateThread::OnTerminate()
{
	this->terminate();
	timer->stop();
	clearList();
	matchPositon = EvaluateThread::NonePositon;
}
void EvaluateThread::OnOver()
{
	emit SigCurrentScore(999, EvaluateThread::NonePositon);
	clearList();
	timer->stop();
}
void EvaluateThread::score()
{
	QList<qreal> leftElbowAngles = caculateSerialsAngle(leftElbow);
	QList<qreal> rightElbowAngles = caculateSerialsAngle(rightElbow);
	QList<qreal> leftArmToShoulderAngles = caculateSerialsAngle(leftArmToShoulder);
	QList<qreal> rightArmToShoulderAngles = caculateSerialsAngle(rightArmToShoulder);
	QList<qreal> leftArmToVerticalAngles = caculateSerialsAngle(leftArmToVertical);
	QList<qreal> rightArmToVerticalAngles = caculateSerialsAngle(rightArmToVertical);
	QList<qreal> leftKneeAngles = caculateSerialsAngle(leftKnee);
	QList<qreal> rightKneeAngles = caculateSerialsAngle(rightKnee);
	QList<qreal> hipRightAngles = caculateSerialsAngle(hipRight);
	QList<qreal> hipLeftAngles = caculateSerialsAngle(hipLeft);
	QList<qreal> crotchAngles = caculateSerialsAngle(crotch);

	QFile file("./distance.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
	{
		qDebug() << "Open Failed!";
	}
	QTextStream in(&file);

	if (index == 0)
	{
		in << "paraNum: " << index << '\n';
		in << "center: " << '\n';

		qreal dis_LeftElbowCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 0],
			leftElbowAngles.mid(0, leftElbowAngles.count() - 30), DYW_R);
		in << dis_LeftElbowCenter << ' ';
		qreal dis_RightElbowCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 1],
			rightElbowAngles.mid(0, rightElbowAngles.count() - 30), DYW_R);
		in << dis_RightElbowCenter << ' ';
		qreal dis_LeftArmToShoulderCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 2],
			leftArmToShoulderAngles.mid(0, leftArmToShoulderAngles.count() - 30), DYW_R);
		in << dis_LeftArmToShoulderCenter << ' ';
		qreal dis_RightArmToShoulderCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 3],
			rightArmToShoulderAngles.mid(0, rightArmToShoulderAngles.count() - 30), DYW_R);
		in << dis_RightArmToShoulderCenter << ' ';
		qreal dis_LeftArmToVerticalCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 4],
			leftArmToVerticalAngles.mid(0, leftArmToVerticalAngles.count() - 30), DYW_R);
		in << dis_LeftArmToVerticalCenter << ' ';
		qreal dis_RightArmToVerticalCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 5],
			rightArmToVerticalAngles.mid(0, rightArmToVerticalAngles.count() - 30), DYW_R);
		in << dis_RightArmToVerticalCenter << ' ';
		qreal dis_LeftKneeCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 6],
			leftKneeAngles.mid(0, leftKneeAngles.count() - 30), DYW_R);
		in << dis_LeftKneeCenter << ' ';
		qreal dis_RightKneeCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 7],
			rightKneeAngles.mid(0, rightKneeAngles.count() - 30), DYW_R);
		in << dis_RightKneeCenter << ' ';
		qreal dis_HipRightCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 8],
			hipRightAngles.mid(0, hipRightAngles.count() - 30), DYW_R);
		in << dis_HipRightCenter << ' ';
		qreal dis_HipLeftCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 9],
			hipLeftAngles.mid(0, hipLeftAngles.count() - 30), DYW_R);
		in << dis_HipLeftCenter << ' ';
		qreal dis_CrotchCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 10],
			crotchAngles.mid(0, crotchAngles.count() - 30), DYW_R);
		in << dis_CrotchCenter << ' ';

		qreal scoreCenter =
			weightList[index][0] * dis_LeftElbowCenter + weightList[index][1] * dis_RightElbowCenter +
			weightList[index][2] * dis_LeftArmToShoulderCenter + weightList[index][3] * dis_RightArmToShoulderCenter +
			weightList[index][4] * dis_LeftArmToVerticalCenter + weightList[index][5] * dis_RightArmToVerticalCenter +
			weightList[index][6] * dis_LeftKneeCenter + weightList[index][7] * dis_RightKneeCenter +
			weightList[index][8] * dis_HipRightCenter + weightList[index][9] * dis_HipLeftCenter +
			weightList[index][10] * dis_CrotchCenter;
		in <<"ave dis:" << scoreCenter << '\n';

		in << "RightOne: " << '\n';

		qreal dis_LeftElbowRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 0],
			leftElbowAngles.mid(15, leftElbowAngles.count() - 30), DYW_R);
		in << dis_LeftElbowRightOne << ' ';
		qreal dis_RightElbowRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 1],
			rightElbowAngles.mid(15, rightElbowAngles.count() - 30), DYW_R);
		in << dis_RightElbowRightOne << ' ';
		qreal dis_LeftArmToShoulderRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 2],
			leftArmToShoulderAngles.mid(15, leftArmToShoulderAngles.count() - 30), DYW_R);
		in << dis_LeftArmToShoulderRightOne << ' ';
		qreal dis_RightArmToShoulderRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 3],
			rightArmToShoulderAngles.mid(15, rightArmToShoulderAngles.count() - 30), DYW_R);
		in << dis_RightArmToShoulderRightOne << ' ';
		qreal dis_LeftArmToVerticalRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 4],
			leftArmToVerticalAngles.mid(15, leftArmToVerticalAngles.count() - 30), DYW_R);
		in << dis_LeftArmToVerticalRightOne << ' ';
		qreal dis_RightArmToVerticalRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 5],
			rightArmToVerticalAngles.mid(15, rightArmToVerticalAngles.count() - 30), DYW_R);
		in << dis_RightArmToVerticalRightOne << ' ';
		qreal dis_LeftKneeRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 6],
			leftKneeAngles.mid(15, leftKneeAngles.count() - 30), DYW_R);
		in << dis_LeftKneeRightOne << ' ';
		qreal dis_RightKneeRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 7],
			rightKneeAngles.mid(15, rightKneeAngles.count() - 30), DYW_R);
		in << dis_RightKneeRightOne << ' ';
		qreal dis_HipRightRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 8],
			hipRightAngles.mid(15, hipRightAngles.count() - 30), DYW_R);
		in << dis_HipRightRightOne << ' ';
		qreal dis_HipLeftRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 9],
			hipLeftAngles.mid(15, hipLeftAngles.count() - 30), DYW_R);
		in << dis_HipLeftRightOne << ' ';
		qreal dis_CrotchRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 10],
			crotchAngles.mid(15, crotchAngles.count() - 30), DYW_R);
		in << dis_CrotchRightOne << ' ';

		qreal scoreRightOne = 
			weightList[index][0] * dis_LeftElbowRightOne + weightList[index][1] * dis_RightElbowRightOne +
			weightList[index][2] * dis_LeftArmToShoulderRightOne + weightList[index][3] * dis_RightArmToShoulderRightOne + 
			weightList[index][4] * dis_LeftArmToVerticalRightOne + weightList[index][5] * dis_RightArmToVerticalRightOne +
			weightList[index][6] * dis_LeftKneeRightOne + weightList[index][7] * dis_RightKneeRightOne + 
			weightList[index][8] * dis_HipRightRightOne + weightList[index][9] * dis_HipLeftRightOne + 
			weightList[index][10] * dis_CrotchRightOne;
		in <<"ave dis:" << scoreRightOne << '\n';

		in << "RightTwo: " << '\n';

		qreal dis_LeftElbowRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 0],
			leftElbowAngles.mid(30, leftElbowAngles.count() - 30), DYW_R);
		in << dis_LeftElbowRightTwo << ' ';
		qreal dis_RightElbowRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 1],
			rightElbowAngles.mid(30, rightElbowAngles.count() - 30), DYW_R);
		in << dis_RightElbowRightTwo << ' ';
		qreal dis_LeftArmToShoulderRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 2],
			leftArmToShoulderAngles.mid(30, leftArmToShoulderAngles.count() - 30), DYW_R);
		in << dis_LeftArmToShoulderRightTwo << ' ';
		qreal dis_RightArmToShoulderRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 3],
			rightArmToShoulderAngles.mid(30, rightArmToShoulderAngles.count() - 30), DYW_R);
		in << dis_RightArmToShoulderRightTwo << ' ';
		qreal dis_LeftArmToVerticalRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 4],
			leftArmToVerticalAngles.mid(30, leftArmToVerticalAngles.count() - 30), DYW_R);
		in << dis_LeftArmToVerticalRightTwo << ' ';
		qreal dis_RightArmToVerticalRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 5],
			rightArmToVerticalAngles.mid(30, rightArmToVerticalAngles.count() - 30), DYW_R);
		in << dis_RightArmToVerticalRightTwo << ' ';
		qreal dis_LeftKneeRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 6],
			leftKneeAngles.mid(30, leftKneeAngles.count() - 30), DYW_R);
		in << dis_LeftKneeRightTwo << ' ';
		qreal dis_RightKneeRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 7],
			rightKneeAngles.mid(30, rightKneeAngles.count() - 30), DYW_R);
		in << dis_RightKneeRightTwo << ' ';
		qreal dis_HipRightRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 8],
			hipRightAngles.mid(30, hipRightAngles.count() - 30), DYW_R);
		in << dis_HipRightRightTwo << ' ';
		qreal dis_HipLeftRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 9],
			hipLeftAngles.mid(30, hipLeftAngles.count() - 30), DYW_R);
		in << dis_HipLeftRightTwo << ' ';
		qreal dis_CrotchRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 10],
			crotchAngles.mid(30, crotchAngles.count() - 30), DYW_R);
		in << dis_CrotchRightTwo << ' ';

		qreal scoreRightTwo = weightList[index][0] * dis_LeftElbowRightTwo + weightList[index][1] * dis_RightElbowRightTwo +
			weightList[index][2] * dis_LeftArmToShoulderRightTwo + weightList[index][3] * dis_RightArmToShoulderRightTwo +
			weightList[index][4] * dis_LeftArmToVerticalRightTwo + weightList[index][5] * dis_RightArmToVerticalRightTwo +
			weightList[index][6] * dis_LeftKneeRightTwo + weightList[index][7] * dis_RightKneeRightTwo +
			weightList[index][8] * dis_HipRightRightTwo + weightList[index][9] * dis_HipLeftRightTwo +
			weightList[index][10] * dis_CrotchRightTwo;
		in << "ave dis:" << scoreRightTwo << '\n';

		qreal minScore;
		int matchPos = minValuePos(scoreCenter, scoreRightOne, scoreRightTwo);
		switch (matchPos)
		{
		case 1:
			minScore = scoreCenter;
			matchPositon = Center;
			break;
		case 2:
			minScore = scoreRightOne;
			matchPositon = RightOne;
			break;
		case 3:
			minScore = scoreRightTwo;
			matchPositon = RightTwo;
			break;
		default:
			minScore = 0.0;
			matchPositon = NonePositon;
			break;
		}
		qDebug() << minScore << "zhangsong";
		int finalscore = getFinalScoreOfQishi(minScore);
		finalscore = finalscore > 100 ? 100 : finalscore;
		in << "score: " << finalscore << '\n' << '\n';
		file.close();

		emit SigCurrentScore(finalscore>100 ? 100 : finalscore, matchPositon);
	}
	else
	{
		in << "paraNum: " << index << '\n';
		in << "LeftTwo: " << '\n';

		bool success = isLowVariance(leftElbowAngles);
		if (success)
			success = isLowVariance(rightElbowAngles);
		if (success)
			success = isLowVariance(leftArmToShoulderAngles);
		if (success)
			success = isLowVariance(rightArmToShoulderAngles);
		if (success)
			success = isLowVariance(leftArmToVerticalAngles);
		if (success)
			success = isLowVariance(rightArmToVerticalAngles);
		if (success)
			success = isLowVariance(rightKneeAngles);
		if (success)
			success = isLowVariance(hipRightAngles);
		if (success)
			success = isLowVariance(hipLeftAngles);
		if (success)
			success = isLowVariance(crotchAngles);
		if (success)
		{
			emit SigCurrentScore(999, EvaluateThread::NonePositon);
			clearList();
			return;
		}
		qreal dis_LeftElbowLeftTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 0],
			leftElbowAngles.mid(0, leftElbowAngles.count() - 60), DYW_R);
		in << dis_LeftElbowLeftTwo << ' ';
		qreal dis_RightElbowLeftTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 1],
			rightElbowAngles.mid(0, rightElbowAngles.count() - 60), DYW_R);
		in << dis_RightElbowLeftTwo << ' ';
		qreal dis_LeftArmToShoulderLeftTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 2],
			leftArmToShoulderAngles.mid(0, leftArmToShoulderAngles.count() - 60), DYW_R);
		in << dis_LeftArmToShoulderLeftTwo << ' ';
		qreal dis_RightArmToShoulderLeftTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 3],
			rightArmToShoulderAngles.mid(0, rightArmToShoulderAngles.count() - 60), DYW_R);
		in << dis_RightArmToShoulderLeftTwo << ' ';
		qreal dis_LeftArmToVerticalLeftTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 4],
			leftArmToVerticalAngles.mid(0, leftArmToVerticalAngles.count() - 60), DYW_R);
		in << dis_LeftArmToVerticalLeftTwo << ' ';
		qreal dis_RightArmToVerticalLeftTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 5],
			rightArmToVerticalAngles.mid(0, rightArmToVerticalAngles.count() - 60), DYW_R);
		in << dis_RightArmToVerticalLeftTwo << ' ';
		qreal dis_LeftKneeLeftTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 6],
			leftKneeAngles.mid(0, leftKneeAngles.count() - 60), DYW_R);
		in << dis_LeftKneeLeftTwo << ' ';
		qreal dis_RightKneeLeftTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 7],
			rightKneeAngles.mid(0, rightKneeAngles.count() - 60), DYW_R);
		in << dis_RightKneeLeftTwo << ' ';
		qreal dis_HipRightLeftTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 8],
			hipRightAngles.mid(0, hipRightAngles.count() - 60), DYW_R);
		in << dis_HipRightLeftTwo << ' ';
		qreal dis_HipLeftLeftTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 9],
			hipLeftAngles.mid(0, hipLeftAngles.count() - 60), DYW_R);
		in << dis_HipLeftLeftTwo << ' ';
		qreal dis_CrotchLeftTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 10],
			crotchAngles.mid(0, crotchAngles.count() - 60), DYW_R);
		in << dis_CrotchLeftTwo << ' ';

		qreal scoreLeftTwo = weightList[index][0] * dis_LeftElbowLeftTwo + weightList[index][1] * dis_RightElbowLeftTwo +
			weightList[index][2] * dis_LeftArmToShoulderLeftTwo + weightList[index][3] * dis_RightArmToShoulderLeftTwo +
			weightList[index][4] * dis_LeftArmToVerticalLeftTwo + weightList[index][5] * dis_RightArmToVerticalLeftTwo +
			weightList[index][6] * dis_LeftKneeLeftTwo + weightList[index][7] * dis_RightKneeLeftTwo +
			weightList[index][8] * dis_HipRightLeftTwo + weightList[index][9] * dis_HipLeftLeftTwo +
			weightList[index][10] * dis_CrotchLeftTwo;
		in << "ave dis:" << scoreLeftTwo << '\n';

		in << "LeftOne: " << '\n';

		qreal dis_LeftElbowLeftOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 0],
			leftElbowAngles.mid(15, leftElbowAngles.count() - 60), DYW_R);
		in << dis_LeftElbowLeftOne << ' ';
		qreal dis_RightElbowLeftOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 1],
			rightElbowAngles.mid(15, rightElbowAngles.count() - 60), DYW_R);
		in << dis_RightElbowLeftOne << ' ';
		qreal dis_LeftArmToShoulderLeftOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 2],
			leftArmToShoulderAngles.mid(15, leftArmToShoulderAngles.count() - 60), DYW_R);
		in << dis_LeftArmToShoulderLeftOne << ' ';
		qreal dis_RightArmToShoulderLeftOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 3],
			rightArmToShoulderAngles.mid(15, rightArmToShoulderAngles.count() - 60), DYW_R);
		in << dis_RightArmToShoulderLeftOne << ' ';
		qreal dis_LeftArmToVerticalLeftOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 4],
			leftArmToVerticalAngles.mid(15, leftArmToVerticalAngles.count() - 60), DYW_R);
		in << dis_LeftArmToVerticalLeftOne << ' ';
		qreal dis_RightArmToVerticalLeftOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 5],
			rightArmToVerticalAngles.mid(15, rightArmToVerticalAngles.count() - 60), DYW_R);
		in << dis_RightArmToVerticalLeftOne << ' ';
		qreal dis_LeftKneeLeftOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 6],
			leftKneeAngles.mid(15, leftKneeAngles.count() - 60), DYW_R);
		in << dis_LeftKneeLeftOne << ' ';
		qreal dis_RightKneeLeftOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 7],
			rightKneeAngles.mid(15, rightKneeAngles.count() - 60), DYW_R);
		in << dis_RightKneeLeftOne << ' ';
		qreal dis_HipRightLeftOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 8],
			hipRightAngles.mid(15, hipRightAngles.count() - 60), DYW_R);
		in << dis_HipRightLeftOne << ' ';
		qreal dis_HipLeftLeftOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 9],
			hipLeftAngles.mid(15, hipLeftAngles.count() - 60), DYW_R);
		in << dis_HipLeftLeftOne << ' ';
		qreal dis_CrotchLeftOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 10],
			crotchAngles.mid(15, crotchAngles.count() - 60), DYW_R);
		in << dis_CrotchLeftOne << ' ';

		qreal scoreLeftOne = weightList[index][0] * dis_LeftElbowLeftOne + weightList[index][1] * dis_RightElbowLeftOne +
			weightList[index][2] * dis_LeftArmToShoulderLeftOne + weightList[index][3] * dis_RightArmToShoulderLeftOne +
			weightList[index][4] * dis_LeftArmToVerticalLeftOne + weightList[index][5] * dis_RightArmToVerticalLeftOne +
			weightList[index][6] * dis_LeftKneeLeftOne + weightList[index][7] * dis_RightKneeLeftOne +
			weightList[index][8] * dis_HipRightLeftOne + weightList[index][9] * dis_HipLeftLeftOne +
			weightList[index][10] * dis_CrotchLeftOne;
		in << "ave dis:" << scoreLeftOne << '\n';


		in << "Center: " << '\n';

		qreal dis_LeftElbowCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 0],
			leftElbowAngles.mid(30, leftElbowAngles.count() - 60), DYW_R);
		in << dis_LeftElbowCenter << ' ';
		qreal dis_RightElbowCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 1],
			rightElbowAngles.mid(30, rightElbowAngles.count() - 60), DYW_R);
		in << dis_RightElbowCenter << ' ';
		qreal dis_LeftArmToShoulderCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 2],
			leftArmToShoulderAngles.mid(30, leftArmToShoulderAngles.count() - 60), DYW_R);
		in << dis_LeftArmToShoulderCenter << ' ';
		qreal dis_RightArmToShoulderCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 3],
			rightArmToShoulderAngles.mid(30, rightArmToShoulderAngles.count() - 60), DYW_R);
		in << dis_RightArmToShoulderCenter << ' ';
		qreal dis_LeftArmToVerticalCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 4],
			leftArmToVerticalAngles.mid(30, leftArmToVerticalAngles.count() - 60), DYW_R);
		in << dis_LeftArmToVerticalCenter << ' ';
		qreal dis_RightArmToVerticalCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 5],
			rightArmToVerticalAngles.mid(30, rightArmToVerticalAngles.count() - 60), DYW_R);
		in << dis_RightArmToVerticalCenter << ' ';
		qreal dis_LeftKneeCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 6],
			leftKneeAngles.mid(30, leftKneeAngles.count() - 60), DYW_R);
		in << dis_LeftKneeCenter << ' ';
		qreal dis_RightKneeCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 7],
			rightKneeAngles.mid(30, rightKneeAngles.count() - 60), DYW_R);
		in << dis_RightKneeCenter << ' ';
		qreal dis_HipRightCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 8],
			hipRightAngles.mid(30, hipRightAngles.count() - 60), DYW_R);
		in << dis_HipRightCenter << ' ';
		qreal dis_HipLeftCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 9],
			hipLeftAngles.mid(30, hipLeftAngles.count() - 60), DYW_R);
		in << dis_HipLeftCenter << ' ';
		qreal dis_CrotchCenter = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 10],
			crotchAngles.mid(30, crotchAngles.count() - 60), DYW_R);
		in << dis_CrotchCenter << ' ';

		qreal scoreCenter = weightList[index][0] * dis_LeftElbowCenter + weightList[index][1] * dis_RightElbowCenter +
			weightList[index][2] * dis_LeftArmToShoulderCenter + weightList[index][3] * dis_RightArmToShoulderCenter +
			weightList[index][4] * dis_LeftArmToVerticalCenter + weightList[index][5] * dis_RightArmToVerticalCenter +
			weightList[index][6] * dis_LeftKneeCenter + weightList[index][7] * dis_RightKneeCenter +
			weightList[index][8] * dis_HipRightCenter + weightList[index][9] * dis_HipLeftCenter +
			weightList[index][10] * dis_CrotchCenter;
		in << "ave dis:" << scoreCenter << '\n';

		in << "RightOne: " << '\n';

		qreal dis_LeftElbowRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 0],
			leftElbowAngles.mid(45, leftElbowAngles.count() - 60), DYW_R);
		in << dis_LeftElbowRightOne << ' ';
		qreal dis_RightElbowRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 1],
			rightElbowAngles.mid(45, rightElbowAngles.count() - 60), DYW_R);
		in << dis_RightElbowRightOne << ' ';
		qreal dis_LeftArmToShoulderRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 2],
			leftArmToShoulderAngles.mid(45, leftArmToShoulderAngles.count() - 60), DYW_R);
		in << dis_LeftArmToShoulderRightOne << ' ';
		qreal dis_RightArmToShoulderRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 3],
			rightArmToShoulderAngles.mid(45, rightArmToShoulderAngles.count() - 60), DYW_R);
		in << dis_RightArmToShoulderRightOne << ' ';
		qreal dis_LeftArmToVerticalRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 4],
			leftArmToVerticalAngles.mid(45, leftArmToVerticalAngles.count() - 60), DYW_R);
		in << dis_LeftArmToVerticalRightOne << ' ';
		qreal dis_RightArmToVerticalRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 5],
			rightArmToVerticalAngles.mid(45, rightArmToVerticalAngles.count() - 60), DYW_R);
		in << dis_RightArmToVerticalRightOne << ' ';
		qreal dis_LeftKneeRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 6],
			leftKneeAngles.mid(45, leftKneeAngles.count() - 60), DYW_R);
		in << dis_LeftKneeRightOne << ' ';
		qreal dis_RightKneeRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 7],
			rightKneeAngles.mid(45, rightKneeAngles.count() - 60), DYW_R);
		in << dis_RightKneeRightOne << ' ';
		qreal dis_HipRightRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 8],
			hipRightAngles.mid(45, hipRightAngles.count() - 60), DYW_R);
		in << dis_HipRightRightOne << ' ';
		qreal dis_HipLeftRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 9],
			hipLeftAngles.mid(45, hipLeftAngles.count() - 60), DYW_R);
		in << dis_HipLeftRightOne << ' ';
		qreal dis_CrotchRightOne = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 10],
			crotchAngles.mid(45, crotchAngles.count() - 60), DYW_R);
		in << dis_CrotchRightOne << ' ';

		qreal scoreRightOne = weightList[index][0] * dis_LeftElbowRightOne + weightList[index][1] * dis_RightElbowRightOne +
			weightList[index][2] * dis_LeftArmToShoulderRightOne + weightList[index][3] * dis_RightArmToShoulderRightOne +
			weightList[index][4] * dis_LeftArmToVerticalRightOne + weightList[index][5] * dis_RightArmToVerticalRightOne +
			weightList[index][6] * dis_LeftKneeRightOne + weightList[index][7] * dis_RightKneeRightOne +
			weightList[index][8] * dis_HipRightRightOne + weightList[index][9] * dis_HipLeftRightOne +
			weightList[index][10] * dis_CrotchRightOne;
		in << "ave dis:" << scoreRightOne << '\n';

		in << "RightTwo: " << '\n';

		qreal dis_LeftElbowRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 0],
			leftElbowAngles.mid(60, leftElbowAngles.count() - 60), DYW_R);
		in << dis_LeftElbowRightTwo << ' ';
		qreal dis_RightElbowRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 1],
			rightElbowAngles.mid(60, rightElbowAngles.count() - 60), DYW_R);
		in << dis_RightElbowRightTwo << ' ';
		qreal dis_LeftArmToShoulderRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 2],
			leftArmToShoulderAngles.mid(60, leftArmToShoulderAngles.count() - 60), DYW_R);
		in << dis_LeftArmToShoulderRightTwo << ' ';
		qreal dis_RightArmToShoulderRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 3],
			rightArmToShoulderAngles.mid(60, rightArmToShoulderAngles.count() - 60), DYW_R);
		in << dis_RightArmToShoulderRightTwo << ' ';
		qreal dis_LeftArmToVerticalRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 4],
			leftArmToVerticalAngles.mid(60, leftArmToVerticalAngles.count() - 60), DYW_R);
		in << dis_LeftArmToVerticalRightTwo << ' ';
		qreal dis_RightArmToVerticalRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 5],
			rightArmToVerticalAngles.mid(60, rightArmToVerticalAngles.count() - 60), DYW_R);
		in << dis_RightArmToVerticalRightTwo << ' ';
		qreal dis_LeftKneeRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 6],
			leftKneeAngles.mid(60, leftKneeAngles.count() - 60), DYW_R);
		in << dis_LeftKneeRightTwo << ' ';
		qreal dis_RightKneeRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 7],
			rightKneeAngles.mid(60, rightKneeAngles.count() - 60), DYW_R);
		in << dis_RightKneeRightTwo << ' ';
		qreal dis_HipRightRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 8],
			hipRightAngles.mid(60, hipRightAngles.count() - 60), DYW_R);
		in << dis_HipRightRightTwo << ' ';
		qreal dis_HipLeftRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 9],
			hipLeftAngles.mid(60, hipLeftAngles.count() - 60), DYW_R);
		in << dis_HipLeftRightTwo << ' ';
		qreal dis_CrotchRightTwo = DTWDistanceFun(taichiDataQreal[index * CHARACT_NUM + 10],
			crotchAngles.mid(60, crotchAngles.count() - 60), DYW_R);
		in << dis_CrotchRightTwo << ' ';

		qreal scoreRightTwo = weightList[index][0] * dis_LeftElbowRightTwo + weightList[index][1] * dis_RightElbowRightTwo +
			weightList[index][2] * dis_LeftArmToShoulderRightTwo + weightList[index][3] * dis_RightArmToShoulderRightTwo +
			weightList[index][4] * dis_LeftArmToVerticalRightTwo + weightList[index][5] * dis_RightArmToVerticalRightTwo +
			weightList[index][6] * dis_LeftKneeRightTwo + weightList[index][7] * dis_RightKneeRightTwo +
			weightList[index][8] * dis_HipRightRightTwo + weightList[index][9] * dis_HipLeftRightTwo +
			weightList[index][10] * dis_CrotchRightTwo;
		in << "ave dis:" << scoreRightTwo << '\n';

		qreal minScore;
		int matchPos = minValuePos(scoreLeftTwo, scoreLeftOne, scoreCenter, scoreRightOne, scoreRightTwo);
		switch (matchPos)
		{
		case 1:
			minScore = scoreLeftTwo;
			matchPositon = LeftTwo;
			break;
		case 2:
			minScore = scoreLeftOne;
			matchPositon = LeftOne;
			break;
		case 3:
			minScore = scoreCenter;
			matchPositon = Center;
			break;
		case 4:
			minScore = scoreRightOne;
			matchPositon = RightOne;
			break;
		case 5:
			minScore = scoreRightTwo;
			matchPositon = RightTwo;
			break;
		default:
			minScore = 0.0;
			matchPositon = NonePositon;
			break;
		}
		//qDebug() << minScore << "zhangsong";
		int finalscore = getFinalScore(minScore);
		finalscore = finalscore > 100 ? 100 : finalscore;
		in << "score: " << finalscore << '\n' << '\n';
		file.close();

		emit SigCurrentScore(finalscore>100 ? 100 : finalscore, matchPositon);
	}
	
	clearList();
	return;
}
void EvaluateThread::clearList()
{
	leftElbow.clear();
	rightElbow.clear();
	leftArmToShoulder.clear();
	rightArmToShoulder.clear();
	leftArmToVertical.clear();
	rightArmToVertical.clear();
	leftKnee.clear();
	rightKnee.clear();
	hipLeft.clear();
	hipRight.clear();
	crotch.clear();
	index = -1;
}
qreal EvaluateThread::DTWDistanceFun(QList<qreal> A, QList<qreal> B, int r)
{
	QList<QList<int>> dtwpath;
	QList<QList<qreal>> distance;
	int i, j;
	qreal dist;
	int istart, imax;
	int r2 = r + qAbs(A.count() - B.count());/*匹配距离*/
	qreal g1, g2, g3;
	int pathsig = 1;/*路径的标志*/

	/*检查参数的有效性*/
	if (A.count()>300 || B.count()>300)
	{
		return -1.0;
	}

	/*进行一些必要的初始化*/
	for (i = 0; i < A.count(); i++)
	{
		QList<qint32> temp_path;
		QList<qreal> temp_dis;
		for (j = 0; j < B.count(); j++)
		{
			temp_path.append(0);
			temp_dis.append(-0.1);
		}
		dtwpath.append(temp_path);
		distance.append(temp_dis);
	}

	/*动态规划求最小距离*/
	distance[0][0] = static_cast<qreal>(2 * qAbs(A[0] - B[0]));
	for (i = 1; i <= r2; i++)
	{
		distance[i][0] = distance[i - 1][0] + qAbs(A[i] - B[0]);
	}
	for (j = 1; j <= r2; j++)
	{
		distance[0][j] = distance[0][j - 1] + qAbs(A[0] - B[j]);
	}

	for (j = 1; j < B.count(); j++)
	{
		istart = j - r2;
		if (j <= r2)
			istart = 1;
		imax = j + r2;
		if (imax >= A.count())
			imax = A.count() - 1;

		for (i = istart; i <= imax; i++)
		{
			g1 = distance[i - 1][j] + qAbs(A[i] - B[j]);
			g2 = distance[i - 1][j - 1] + 2 * qAbs(A[i] - B[j]);
			g3 = distance[i][j - 1] + qAbs(A[i] - B[j]);
			g2 = qMin(g1, g2);
			g3 = qMin(g2, g3);
			distance[i][j] = g3;
		}
	}

	dist = distance[A.count() - 1][B.count() - 1] / static_cast<qreal>(A.count() + B.count());
	return dist;
}
template <class Type>
void EvaluateThread::readTxt(QString fileName, QList<QList<Type>> &list)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Open Failed!";
	}
	QStringList taichiDataStr;
	while (!file.atEnd())
	{
		QByteArray line = file.readLine();
		QString str(line);
		taichiDataStr.append(str);
		//qDebug() << str;
	}
	for (int i = 0; i < taichiDataStr.count(); i++)
	{
		QStringList tempsl = taichiDataStr[i].split(" ");
		QList<qreal> tempdata;
		for (int j = 0; j < tempsl.count(); j++)
		{
			if (tempsl.count() >11)
			{
				if (tempsl[j].toDouble() != 0)
					tempdata.append(tempsl[j].toDouble());
			}
			else
			{
				tempdata.append(tempsl[j].toDouble());
			}
		}
		list.append(tempdata);
	}
	file.close();
}
int EvaluateThread::minValuePos(qreal value1, qreal value2, qreal value3)
{
	int minPos = 0;
	qreal tempValue;
	if (value1 < value2)
	{
		minPos = 1;
		tempValue = value1;
	}
	else
	{
		minPos = 2;
		tempValue = value2;
	}
	if (tempValue < value3)
	{
		return minPos;
	}
	else
	{
		minPos = 3;
		return minPos;
	}

}
int EvaluateThread::minValuePos(qreal value1, qreal value2, qreal value3, qreal value4, qreal value5)
{
	qreal minValueInTop3;
	qreal temp = value1 < value2 ? value1 : value2;
	minValueInTop3 = temp < value3 ? temp : value3;
	int minPos123 = minValuePos(value1, value2, value3);
	int minPos45 = minValuePos(minValueInTop3, value4, value5);
	if (minPos45 == 1)
	{
		return minPos123;
	}
	else if (minPos45 == 2)
	{
		return 4;
	}
	else if (minPos45 == 3)
	{
		return 5;
	}
	else
	{
		return -1;
	}
}
int EvaluateThread::getFinalScore(qreal disFromDtw)
{
	return (1100 - (disFromDtw * 1000 - 200)) / 1100 * 100;
}
int EvaluateThread::getFinalScoreOfQishi(qreal disFromDtw)
{
	return (900 - (disFromDtw * 1000 - 100)) / 900 * 100;
}
qreal EvaluateThread::ave(QList<qreal> list)
{
	qreal sum = 0; 
	for (int i = 0; i < list.count(); i++)
		sum += list[i];
	return sum / list.count();
}
qreal EvaluateThread::variance(QList<qreal> list)
{
	qreal sum = 0;
	qreal average = ave(list);
	for (int i = 0; i < list.count(); i++)
		sum += (list[i] - average)*(list[i] - average);
	return sum / list.count();
}
bool EvaluateThread::isLowVariance(QList<qreal> list)
{
	qDebug() << variance(list) << "variance";
	if (variance(list) < 20)
		return true;
	else
		return false;
}