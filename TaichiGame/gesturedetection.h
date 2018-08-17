#ifndef GESTUREDETECTION_H
#define GESTUREDETECTION_H
#include <Kinect.h>
#include <ctime>  
#include <cassert>  
#include <process.h>  
#include "stdafx.h"

// 记录手势当前位置  
enum GesturePos{
	NonePos = 0,
	Left,
	Right,
	Neutral
};
// 判断识别状态  
enum DetectionState{
	NoneState = 0,
	Success,
	Failed,
	InProgress
};   
// 判断手势需要的数据  
struct DataState{
	GesturePos Pos;     // 每个人的左右手
	DetectionState State; // 状态
	int times;
	time_t timestamp;
	void Reset() // 状态的重置
	{
		Pos = GesturePos::NonePos;
		State = DetectionState::NoneState;
		times = 0;
		timestamp = 0;
	}
};
// 完成手势判断逻辑功能
class GestureDetection
{
public:
	GestureDetection(float neutral_threshold, int times, double difftimes);
	~GestureDetection();

	// 功能：循环接收骨骼数据，如果识别出为挥手动作则输出：success，识别失败输出：failed，
	void Update(IBody * frame);
private:
	// 判断当前的状态成功输出：success，并生成事件：DetectionEvent 
	// 失败输出 failed，供 UpDate 函数调用
	void JudgeState(IBody *n_body, DataState& data, bool isLeft);

	bool IsLeftSide(float curpos, float center);

	bool IsRightSide(float curpos, float center);

	bool IsNeutral(float curpos, float center); // 参数分别为手部的位置和肘部的位置  判断是否是中立的状态

	int get_length(float shou, float zhou);   //  在这里定义  右边是 1 左边是 -1 中间是 0
	
	// 判断骨骼追踪情况：包括骨骼追踪完好且手部位置在肘上面  
	bool IsSkeletonTrackedWell(IBody *n_body, bool isLeft);
private:
	DataState wave_datas[1][2];        // 记录每个人，每只手的状态
	const int left_hand;                            // 左手 ID
	const int right_hand;                           // 右手 ID
	// 中间位置阀值：在该范围内的都认为手在中间位置（相对于肘部的 x 坐标）
	const float neutral_threshold;
	// 挥手次数阀值，达到该次数认为是挥手  
	const int times;
	// 时间限制，如果超过该时间差依然识别不出挥手动作则认为识别失败  
	const double difftimes;

	friend class TaichiGame;
};
#endif