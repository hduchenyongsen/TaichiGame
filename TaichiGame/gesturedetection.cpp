#include "gesturedetection.h"
#include <QDebug>

static int CTRL = 0;

GestureDetection::GestureDetection(float neutral_threshold, int times, double difftimes)
	: neutral_threshold(neutral_threshold)
	, times(times)
	, difftimes(difftimes)
	, left_hand(0)
	, right_hand(1)
{
	for (int i = 0; i < 1; i++)
	{
		wave_datas[i][left_hand].Reset();
		wave_datas[i][right_hand].Reset();
	}
}
GestureDetection::~GestureDetection()
{

}
void GestureDetection::Update(IBody * frame)
{
	if (NULL == frame)
		return;
	for (int i = 0; i < 1; i++)
	{
		JudgeState(frame, wave_datas[i][right_hand], true);
	}
}
void GestureDetection::JudgeState(IBody *n_body, DataState& data, bool isLeft)
{

	Joint joints[JointType_Count]; // 定义骨骼信息

	n_body->GetJoints(JointType::JointType_Count, joints);  //获取骨骼信息节点

	int elbow = JointType_ElbowRight;

	int hand = JointType_HandRight;

	if (!IsSkeletonTrackedWell(n_body, isLeft))  //如果手部的位置在肘部之上则认为为真
	{
		if (data.State == InProgress)
		{
			data.Reset();
			return;
		}
	}
	float curpos = joints[hand].Position.X;
	float center = joints[elbow].Position.X;  //  得到人手部和肘部的X坐标的位置  都是右手
	if (!IsNeutral(curpos, center))  //  如果手部不是在中立的位置
	{
		if (data.Pos == NonePos)
		{
#ifdef _DDEBUG
			qDebug() << "found!\n";
#endif
			data.times++;
			if (get_length(curpos, center) == -1)
			{
				data.Pos = Left;
			}
			else if (get_length(curpos, center) == 1)
			{
				data.Pos = Right;
			}
			//qDebug() << "times:" << data.times;
			if (data.Pos == Left)
			{
				qDebug() << "left !\n";
			}
			else if (data.Pos == Right)
			{
				qDebug() << "right!\n";
			}
			else
				qDebug() << "you can't see me!\n";

			data.State = InProgress;
			data.timestamp = time(NULL);
		}
		else if (((data.Pos == Left) && get_length(curpos, center) == 1) || ((data.Pos == Right) && get_length(curpos, center) == -1))  // 左摆找右摆  右摆找左摆
		{
			assert(data.State == InProgress);
			data.times++;
			data.Pos = (data.Pos == Left) ? Right : Left;
 
			qDebug() << "times:" << data.times << endl;
			if (data.Pos == Left)
			{
				qDebug() << "left !\n";
			}
			else if (data.Pos == Right)
			{
				qDebug() << "right!\n";
			}
			else
				qDebug() << "you can't see me!\n";
 
			if (data.times >= times)
			{
				qDebug() << "success!\n";
				CTRL = 1;
				data.Reset();
			}
			else if (difftime(time(NULL), data.timestamp) > difftimes)
			{
				qDebug() << "time out, detection failed!\n";
				qDebug() << "data.times : " << data.times << endl;
				data.Reset();
			}
		}
	}
}
bool GestureDetection::IsLeftSide(float curpos, float center)
{
	int i = 0;
	i = get_length(curpos, center);
	if (i == -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool GestureDetection::IsRightSide(float curpos, float center)
{
	int i = 0;
	i = get_length(curpos, center);
	if (i == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool GestureDetection::IsNeutral(float curpos, float center) // 参数分别为手部的位置和肘部的位置  判断是否是中立的状态
{
	int i = 0;
	i = get_length(curpos, center);
	if (i == 0)
	{
		return true;  // 是中立的状态
	}
	else
	{
		return false;
	}
}
int GestureDetection::get_length(float shou, float zhou)   //  在这里定义  右边是 1 左边是 -1 中间是 0
{
	if (shou >= 0 && zhou >= 0)
	{
		if ((shou - zhou) > neutral_threshold)
		{
			return 1; // 在右边
		}
		else if ((shou - zhou) < neutral_threshold || (zhou - shou) > -neutral_threshold)
		{
			return 0;  // 中立
		}
		else
		{
			return -1;   // 左边
		}
	}
	else if (shou >= 0 && zhou <= 0)
	{
		if ((shou + (-zhou)) > neutral_threshold)
		{
			return 1; // 右边
		}
		else
		{
			return 0; // 中立
		}
	}
	else if (shou <= 0 && zhou >= 0)
	{
		if (((-shou) + zhou) > neutral_threshold)
		{
			return -1; // 左边
		}
		else
		{
			return 0; // 中立
		}
	}
	else
	{
		if ((-shou) >= (-zhou))
		{
			if (((-shou) + zhou) > neutral_threshold)
			{
				return -1; // 左边
			}
			else if (((-shou) + zhou) < neutral_threshold)
			{
				return 0; // 中立
			}
			else
			{
				return 1;  // 右边
			}
		}
		else
		{
			if (((-zhou) + shou) > neutral_threshold)
			{
				return 1; // 右边
			}
			else
			{
				return 0; // 中立
			}
		}
	}
}
// 判断骨骼追踪情况：包括骨骼追踪完好且手部位置在肘上面  
bool GestureDetection::IsSkeletonTrackedWell(IBody *n_body, bool isLeft)
{
	Joint joints[JointType_Count];
	n_body->GetJoints(JointType::JointType_Count, joints);
	int elbow = JointType_ElbowRight;
	int hand = JointType_HandRight;
	if (joints[hand].Position.Y > joints[elbow].Position.Y)
	{
		return true;
	}
	else
	{
		return false;
	}
}