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
	//��ȡһ�����������ݣ�75��
	void OnGetData(int _index, QList<QList<int>> data);
	void OnTerminate();
	void OnOver();
protected:
	void run();
private:
	//���������ռ����꣬�ڶ���Ϊ���㣬����ռ�Ƕ�
	qreal calculateAngle(Point3D pointA, Point3D pointB_Peak, Point3D pointC);
	//�����γ�һ���Ƕȵ������������仯���ݣ�9�У����õ�����Ƕȵı仯����
	QList<double> caculateSerialsAngle(QList<QList<int>> data);
	//��ȡ9�����ݣ�������3�����x�����ݵ�������ע��pointB��peak��
	QList<QList<int>> get9ColumnData(QList<QList<int>> allData, int pointABegin, int pointBBegin, int pointCBegin);
	//����÷�
	void score();
	//�������
	void clearList();
	//dtw��ʵ��
	qreal DTWDistanceFun(QList<qreal> A, QList<qreal> B, int r);

	template <class Type>
	void readTxt(QString fileName, QList<QList<Type>> &list);

	int minValuePos(qreal value1, qreal value2, qreal value3, qreal value4, qreal value5); //������Сֵ�ı��
	int minValuePos(qreal value1, qreal value2, qreal value3);
	int getFinalScore(qreal disFromDtw);
	qreal ave(QList<qreal> list);
	qreal variance(QList<qreal> list);
	bool isLowVariance(QList<qreal> list);
	int getFinalScoreOfQishi(qreal disFromDtw);
	//QList<QList<int>> evaluatingData;     //��¼��ȡ����һ�����������ݣ�75��

	QList<QList<int>> leftElbow;          //��¼����ؽ��γɵĽǶȵ�������仯��9��
	QList<QList<int>> rightElbow;         //��¼����ؽ��γɵĽǶȵ�������仯��9��
	QList<QList<int>> leftArmToShoulder;  //��¼���ϱ������γɵĽǶȵ�������仯��9��
	QList<QList<int>> rightArmToShoulder; //��¼���ϱ�����ˮƽ�����γɵĽǶȵ�������仯��9��
	QList<QList<int>> leftArmToVertical;  //��¼���ϱ��봹ֱ�����γɵĽǶȵ�������仯��9��
	QList<QList<int>> rightArmToVertical; //��¼���ϱ��봹ֱ�����γɵĽǶȵ�������仯��9��
	QList<QList<int>> leftKnee;           //��¼��ϥ�ؽ��γɵĽǶȵ�������仯��9��
	QList<QList<int>> rightKnee;          //��¼��ϥ�ؽ��γɵĽǶȵ�������仯��9��
	QList<QList<int>> hipRight;           //��¼�Źؽ��γɵĽǶȵ�������仯-���������ȣ�9��
	QList<QList<int>> hipLeft;            //��¼�Źؽ��γɵĽǶȵ�������仯-���������ȣ�9��
	QList<QList<int>> crotch;             //��¼˫������Ƕȣ�9��

	int index; //������̫��������Ϊ���ɶΣ�index��¼��ǰ�������۵�����һ������
	MatchPosition matchPositon;
	QList<QList<qreal>> weightList;   //����Ȩֵ
};

#endif // EVALUATETHREAD_H
