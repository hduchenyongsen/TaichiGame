#ifndef PLAYBACKRECORDTHREAD_H
#define PLAYBACKRECORDTHREAD_H

#include <QThread>
#include <windef.h >
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

class PlayBackRecordThread : public QThread
{
	Q_OBJECT

public:
	PlayBackRecordThread(QObject *parent);
	~PlayBackRecordThread();
public slots:
	void OnRecieveFrameData(BYTE* frame);
protected:
	void run(); 
private:
	void releaseVideoWriter();
	void openVideoFile();
	BYTE *currentFrame;
	int currentPlayedTime;
	cv::VideoWriter videoWriter;
	bool GetFrameData;
	//cv::Mat maskImage;

	friend class TaichiGame;
};

#endif // PLAYBACKRECORDTHREAD_H
