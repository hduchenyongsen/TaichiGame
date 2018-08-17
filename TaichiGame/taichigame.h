#ifndef TAICHIGAME_H
#define TAICHIGAME_H

#include <QtWidgets/QWidget>
#include "ui_taichigame.h"
#include <QTimer>
#include <QDebug>
#include <QApplication>
#include <QtGui>
#include <QLabel>
#include <QSlider>
#include <QStackedWidget>
#include <QMediaPlayer>
#include <QList>
#include "Kinect.h"
#include <QVideoWidget>
#include <QFile>
#include <QDir>
#include <QPushButton>
#include <QProgressBar>
#include <QMediaPlaylist>
#include <QStandardItemModel>
#include <QTableView>
#include <QProgressBar>
#include "labelwithborder.h"
#include "gesturedetection.h"
#include "evaluatethread.h"
#include "database.h"
#include "playbackrecordthread.h"

#include"stdafx.h"

class TaichiGame : public QWidget
{
	Q_OBJECT

public:
	TaichiGame(QWidget *parent = 0);
	~TaichiGame();
	enum MidLabelState
	{
		Beginning,
		Selection,
		Demonstration,
		Scoring,
		None
	};
	enum PlayMode
	{
		WihtDemonstration_Left,
		WihtoutDemonstration_Right,
		NoMode
	};
	enum ScoreSection
	{
		NoScore,
		LessThan55,
		From55To65,
		From65To75,
		From75To85,
		From85To95,
		MoreThan95
	};
	enum HandPosForLastState
	{
		HandUpLeft,
		HandUpRight,
		HandDown
	};
signals:
	void SigTransferData(int index, QList<QList<int>> data); //index为当前发送的数据段标记
private slots:
	void OnReadFrame();
	void OnSwitch();
	void OnSolveDemoMediaState(QMediaPlayer::MediaStatus status);
	void OnSolvePlayBackMediaState(QMediaPlayer::MediaStatus status);
	void OnToTheVeryBeginning();
	void OnUpdateCurrentScore(int score, EvaluateThread::MatchPosition pos);
	void OnMediaPositionChanged(qint64 positon);
	void OnMoveSlider();
	void OnMoveProgBar();
	void OnReleaseVideoWriter();
protected:
	void mouseReleaseEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);
private:
	void InitUi();
	void InitOthers();
	void InitConnect();
	void updateColorData();
	void updateDepthData();
	void updateBodyData();
	HRESULT init();
	void writeDataBase(int score);
	void readDataBaseAndShow();
	void showScoreOfThisTime(int score, QList<int> scoreRem);
	void mediaPlayOrStop();
	QImage cvMat2QImage(const cv::Mat &mat);
	void playBack();
	void recordBodyData();
	int toMSeconds(int paraLenth);
	void showRespScore(QList<int> scoreRem);

	static const int cColorWidth = 1920;
	static const int cColorHeight = 1080; 
	static const int cDepthWidth = 512;
	static const int cDepthHeight = 424;

	Ui::TaichiGameClass ui;
	IKinectSensor*      m_pKinectSensor;		// Current Kinect  
	ICoordinateMapper*  m_pCoordinateMapper;
	IColorFrameReader*  m_pColorFrameReader;	// Color reader  
	IDepthFrameReader*  m_pDepthFrameReader;	// Depth reader
	IBodyFrameReader*   m_pBodyFrameReader;		//body reader
	RGBQUAD*            m_pColorRGBX;
	uchar *pDest;
	uchar *pSrc;
	MidLabelState midLabelState;
	PlayMode selectedMode;
	ScoreSection currentScore;
	HandPosForLastState handPosLastState;
	GestureDetection *gesture;
	HandState leftHandStateForDetection;
	HandState rightHandStateForDetection;

	QTimer *sliderTimer;
	QTimer *progBarTimer;
	QTimer *readTimer;
	QTimer *beginTimer;
	QTimer *scoreTimer;
	QTimer *releaseTimer;
	QLabel *beginMaskLabel;
	QLabel *beginLabel;
	QFrame *selectionFrame;
	QLabel *practiceRedLabel;
	QLabel *challengeRedLabel;
	QLabel *practiceGrayLabel;
	QLabel *challengeGrayLabel;
	QLabel *practiceSelectedLabel;
	QLabel *challengeSelectedLabel;
	QVideoWidget *demostrationWidget;
	LabelWithBorder *scoreWidget;
	QLabel *thistimescoreHint;
	QLabel *thisTimeScoreLabel;
	QLabel *leftFourWords;
	QLabel *rightFourWords;
	QLabel *juanhongScoreLabel;
	QLabel *louxiScoreLabel;
	QLabel *yemaScoreLabel;
	QLabel *yunshouScoreLabel;
	QLabel *jinjiScoreLabel;
	QLabel *dengjiaoScoreLabel;
	QLabel *lanqueweiScoreLabel;
	QLabel *shizishouScoreLabel;
	QLabel *historyScoreHintLabel;
	QStandardItemModel *historyScoreModel;
	QTableView *historyScoreview;
	QStackedWidget *midStackedWidget;
	QLabel *realSceneMaskLabel;
	QLabel *realTimeSceneLabel;		//实时场景label
	QLabel *playBackLabelIn;
	QLabel *playBackLabelOut;
	QLabel *returnLabelIn;
	QLabel *returnLabelOut;
	QLabel *playBackSelectedLabel;
	QLabel *returnSelectedLabel;
	QMediaPlayer demoVideoPlayer;
	QMediaPlaylist bgmList;
	QMediaPlayer bgmAllTheWay;
	QMediaPlayer modeSelectionSound;
	QMediaPlayer playBackVideo;
	QVideoWidget *playBackWidget;
	QLabel *recLabel;
	LabelWithBorder *remindWidget;			//下方的提示label
	QLabel *remindLabel;
	//QLabel *remind
	QLabel *scoreBarLabel;	
	QProgressBar *currentScoreProgBar;
	QLabel *scoreArtworLabel;
	QLabel *scoreSliderLabel;
	QLabel *rhythmArtworLabel;
	QSlider *rhythmSlider;			//显示动作快慢
	QPushButton *closeButton;
	//QProgressBar *scoreBar;
	QList<QList<qint32>> paragraphsData;
	QList<QList<qint32>> lastParagraphsData;
	QList<QList<qint32>> headData;
	QList<QList<qint32>> tailData;
	bool tailDataRecording;
	int paraLength;
	QList<int> paraLengthTable;
	EvaluateThread *evaluateThread;
	PlayBackRecordThread *recordThread;
	bool BeginTimerStarted;
	int targetPosForSlider;
	int paragraphIndex;
	bool isTaichiPlaying;
	QList<int> scoreRemember;
	QList<EvaluateThread::MatchPosition> rhythmRemember;
	int finalscoretoshow;    //保存显示在屏幕上的最终得分
	QList<int> handCloseCounting;
	bool coutingBegan;
	DataBase historyScoreDataBase;
	QImage backImg;
	int progBarTargetValue;
	int bodyIndex;
	bool ToStartRecordVideo;
	bool recordVideoStarted;
	bool isPlayingBack;
	QList<QList<int>> bodyDataToRecord;
	int cuurentPlayBackParaNo;   //记录当前正在回放的段落编号
	int paraLenthShowed;    //记录已经回放过的动作帧数
	int frameNum; //评价时数帧数，帧数为9的倍数就剔除，从而调整帧率为26-27
	qreal lastFrameDis;
	bool dataFilterStarted;

	QLabel *tempLabel;
};

#endif // TAICHIGAME_H
