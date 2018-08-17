#include "playbackrecordthread.h"
#include <QDebug>
using namespace cv;

PlayBackRecordThread::PlayBackRecordThread(QObject *parent)
	: QThread(parent),
	GetFrameData(false)
{
	currentFrame = new BYTE();
	//maskImage = imread(":/TaichiGame/Resources/rec.png");
	//cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX,1.0,1.0,0,2,8);
}

PlayBackRecordThread::~PlayBackRecordThread()
{

}

void PlayBackRecordThread::OnRecieveFrameData(BYTE* frame)
{
	currentFrame = frame;
	GetFrameData = true;
	this->start();
}

void PlayBackRecordThread::run()
{
	if (GetFrameData)
	{
		Mat frame(1080, 1920, CV_8UC4, currentFrame);
		Mat temp = Mat::zeros(640, 360, CV_8UC4);//需要分析的图片
		cv::resize(frame, temp, Size(640, 360), 0, 0, INTER_CUBIC);
		Mat cvColorImg = Mat::zeros(640, 360, CV_8UC4);
		// cvtColor(temp, cvColorImg, CV_RGB2BGR);  // CV_RGB2BGR参数会导致颜色变化，原因是RGB变成BGR
		cvtColor(temp, cvColorImg, CV_BGRA2BGR);
		Mat imageROI = cvColorImg(Rect(0, 0, cvColorImg.cols, cvColorImg.rows));
		//maskImage.copyTo(imageROI, cvColorImg);
		//addWeighted(cvColorImg, 0.5, maskImage, 0.5, 0, cvColorImg);
		putText(cvColorImg, "Rec", Point(510, 40), FONT_HERSHEY_COMPLEX, 0.9, cv::Scalar(0, 0, 192),2,8);
		circle(cvColorImg, Point(582, 33), 3, cv::Scalar(0, 0, 192),-1);
		circle(cvColorImg, Point(582, 33), 9, cv::Scalar(0, 0, 192), 3);
		videoWriter << cvColorImg;
		GetFrameData = false;
	}
	return;
}
void PlayBackRecordThread::openVideoFile()
{
	videoWriter.open("./PlayBack/playbackVideo.avi", CV_FOURCC('X', 'V', 'I', 'D'), 30, cvSize(640, 360), 1);
}
void PlayBackRecordThread::releaseVideoWriter()
{
	if (videoWriter.isOpened())
		videoWriter.release();
}

