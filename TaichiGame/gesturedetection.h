#ifndef GESTUREDETECTION_H
#define GESTUREDETECTION_H
#include <Kinect.h>
#include <ctime>  
#include <cassert>  
#include <process.h>  
#include "stdafx.h"

// ��¼���Ƶ�ǰλ��  
enum GesturePos{
	NonePos = 0,
	Left,
	Right,
	Neutral
};
// �ж�ʶ��״̬  
enum DetectionState{
	NoneState = 0,
	Success,
	Failed,
	InProgress
};   
// �ж�������Ҫ������  
struct DataState{
	GesturePos Pos;     // ÿ���˵�������
	DetectionState State; // ״̬
	int times;
	time_t timestamp;
	void Reset() // ״̬������
	{
		Pos = GesturePos::NonePos;
		State = DetectionState::NoneState;
		times = 0;
		timestamp = 0;
	}
};
// ��������ж��߼�����
class GestureDetection
{
public:
	GestureDetection(float neutral_threshold, int times, double difftimes);
	~GestureDetection();

	// ���ܣ�ѭ�����չ������ݣ����ʶ���Ϊ���ֶ����������success��ʶ��ʧ�������failed��
	void Update(IBody * frame);
private:
	// �жϵ�ǰ��״̬�ɹ������success���������¼���DetectionEvent 
	// ʧ����� failed���� UpDate ��������
	void JudgeState(IBody *n_body, DataState& data, bool isLeft);

	bool IsLeftSide(float curpos, float center);

	bool IsRightSide(float curpos, float center);

	bool IsNeutral(float curpos, float center); // �����ֱ�Ϊ�ֲ���λ�ú��ⲿ��λ��  �ж��Ƿ���������״̬

	int get_length(float shou, float zhou);   //  �����ﶨ��  �ұ��� 1 ����� -1 �м��� 0
	
	// �жϹ���׷���������������׷��������ֲ�λ����������  
	bool IsSkeletonTrackedWell(IBody *n_body, bool isLeft);
private:
	DataState wave_datas[1][2];        // ��¼ÿ���ˣ�ÿֻ�ֵ�״̬
	const int left_hand;                            // ���� ID
	const int right_hand;                           // ���� ID
	// �м�λ�÷�ֵ���ڸ÷�Χ�ڵĶ���Ϊ�����м�λ�ã�������ⲿ�� x ���꣩
	const float neutral_threshold;
	// ���ִ�����ֵ���ﵽ�ô�����Ϊ�ǻ���  
	const int times;
	// ʱ�����ƣ����������ʱ�����Ȼʶ�𲻳����ֶ�������Ϊʶ��ʧ��  
	const double difftimes;

	friend class TaichiGame;
};
#endif