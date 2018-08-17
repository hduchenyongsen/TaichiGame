#ifndef EVALUATETHREAD_H
#define EVALUATETHREAD_H

#include <QThread>
#include <QStringList>
#include <QTimer>

class EvaluateThread : public QThread
{
	Q_OBJECT

public:
	typedef struct _Point3D
	{
		int x;
		int y;
		int z;
	}Point3D;
	enum JointType
	{
		SpineBase = 0,
		SpineMid = 1,
		Neck = 2,
		Head = 3,
		ShoulderLeft = 4,
		ElbowLeft = 5,
		WristLeft = 6,
		HandLeft = 7,
		ShoulderRight = 8,
		ElbowRight = 9,
		WristRight = 10,
		HandRight = 11,
		HipLeft = 12,
		KneeLeft = 13,
		AnkleLeft = 14,
		FootLeft = 15,
		HipRight = 16,
		KneeRight = 17,
		AnkleRight = 18,
		FootRight = 19,
		SpineShoulder = 20,
		HandTipLeft = 21,
		ThumbLeft = 22,
		HandTipRight = 23,
		ThumbRight = 24,
		JointCount = (ThumbRight + 1)
	};
	enum MatchPosition
	{
		NonePositon,
		LeftTwo,
		LeftOne,
		Center,
		RightOne,
		RightTwo
	};
	EvaluateThread(QObject *parent);
	~EvaluateThread();
	QTimer *timer;
signals:
	void SigCurrentScore(int score, EvaluateThread::MatchPosition pos);
public slots:
	//获取一段完整的数据，75列
	void OnGetData(int _index, QList<QList<int>> data);
	void OnTerminate();
	void OnOver();
protected:
	void run();
private:
	//给出三个空间坐标，第二个为顶点，计算空间角度
	qreal calculateAngle(Point3D pointA, Point3D pointB_Peak, Point3D pointC);
	//传入形成一个角度的三个点的坐标变化数据（9列），得到这个角度的变化数组
	QList<double> caculateSerialsAngle(QList<QList<int>> data);
	//获取9列数据，按序传入3个点的x轴数据的列数，注意pointB是peak点
	QList<QList<int>> get9ColumnData(QList<QList<int>> allData, int pointABegin, int pointBBegin, int pointCBegin);
	//计算得分
	void score();
	//清空数据
	void clearList();
	//dtw的实现
	qreal DTWDistanceFun(QList<qreal> A, QList<qreal> B, int r);

	template <class Type>
	void readTxt(QString fileName, QList<QList<Type>> &list);

	int minValuePos(qreal value1, qreal value2, qreal value3, qreal value4, qreal value5); //返回最小值的编号
	int minValuePos(qreal value1, qreal value2, qreal value3);
	int getFinalScore(qreal disFromDtw);
	qreal ave(QList<qreal> list);
	qreal variance(QList<qreal> list);
	bool isLowVariance(QList<qreal> list);
	int getFinalScoreOfQishi(qreal disFromDtw);
	//QList<QList<int>> evaluatingData;     //记录获取到的一段完整的数据，75列

	QList<QList<int>> leftElbow;          //记录左肘关节形成的角度的三个点变化，9列
	QList<QList<int>> rightElbow;         //记录右肘关节形成的角度的三个点变化，9列
	QList<QList<int>> leftArmToShoulder;  //记录左上臂与肩膀形成的角度的三个点变化，9列
	QList<QList<int>> rightArmToShoulder; //记录右上臂与肩膀（水平方向）形成的角度的三个点变化，9列
	QList<QList<int>> leftArmToVertical;  //记录左上臂与垂直方向形成的角度的三个点变化，9列
	QList<QList<int>> rightArmToVertical; //记录右上臂与垂直方向形成的角度的三个点变化，9列
	QList<QList<int>> leftKnee;           //记录左膝关节形成的角度的三个点变化，9列
	QList<QList<int>> rightKnee;          //记录右膝关节形成的角度的三个点变化，9列
	QList<QList<int>> hipRight;           //记录髋关节形成的角度的三个点变化-躯干与右腿，9列
	QList<QList<int>> hipLeft;            //记录髋关节形成的角度的三个点变化-躯干与左腿，9列
	QList<QList<int>> crotch;             //记录双腿劈叉角度，9列

	int index; //将整个太极动作分为若干段，index记录当前正在评价的是哪一段数据
	MatchPosition matchPositon;
	QList<QList<qreal>> weightList;   //保存权值
};

#endif // EVALUATETHREAD_H
