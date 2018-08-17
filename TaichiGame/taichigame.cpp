#include "taichigame.h"
#include "gesturedetection.h"
#include <QLayout>
#include <QMetaType>
using namespace cv;

TaichiGame::TaichiGame(QWidget *parent)
	: QWidget(parent),
	midLabelState(Beginning),
	leftHandStateForDetection(HandState_Unknown),
	rightHandStateForDetection(HandState_Unknown),
	demoVideoPlayer(0, QMediaPlayer::VideoSurface),
	BeginTimerStarted(false),
	targetPosForSlider(0),
	paragraphIndex(0),
	isTaichiPlaying(false),
	finalscoretoshow(0),
	coutingBegan(false),
	tailDataRecording(false),
	currentScore(NoScore),
	handPosLastState(HandDown),
	progBarTargetValue(0),
	bodyIndex(0),
	ToStartRecordVideo(false),
	recordVideoStarted(false),
	isPlayingBack(false),
	cuurentPlayBackParaNo(0),
	frameNum(0),
	lastFrameDis(0.0),
	dataFilterStarted(false)
{
	backImg.load(":/TaichiGame/Resources/backGround.jpg");
	InitUi();
	InitOthers();
	InitConnect();
}
TaichiGame::~TaichiGame()
{
	if (m_pColorRGBX)
	{
		delete[] m_pColorRGBX;
		m_pColorRGBX = NULL;
	}

	SafeRelease(m_pColorFrameReader);// done with color frame reader
	SafeRelease(m_pDepthFrameReader);
	SafeRelease(m_pBodyFrameReader);
	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();// close the Kinect Sensor
	}
	SafeRelease(m_pKinectSensor);
	SafeRelease(m_pCoordinateMapper);
}
void TaichiGame::InitUi()
{
	ui.setupUi(this);
	this->resize(1080,1920);
	setWindowFlags(Qt::FramelessWindowHint);

	tempLabel = new QLabel;
	tempLabel->resize(500, 500);
	tempLabel->move(100, 100);
	//tempLabel->show();

	QPushButton *button = new QPushButton(tr("Switch")); 
	button->resize(300,300);
	button->move(100,650);
	//button->show();
	connect(button, SIGNAL(clicked()), this, SLOT(OnSwitch()));

	beginLabel = new QLabel(this);
	beginLabel->resize(1080,607);
	beginLabel->setPixmap(QPixmap(":/TaichiGame/Resources/kaishitishi.png").scaledToWidth(800));
	beginLabel->setAlignment(Qt::AlignCenter);

	selectionFrame = new QFrame(this);
	selectionFrame->resize(1080,607);
	//selectionFrame->setStyleSheet(".border: 1px solid #981a1e;");

	practiceRedLabel = new QLabel(selectionFrame);
	practiceRedLabel->resize(250, 250);
	practiceRedLabel->setScaledContents(true);
	practiceRedLabel->setPixmap(QPixmap(":/TaichiGame/Resources/practiceMode_red.png"));
	practiceRedLabel->move(216, 194);
	practiceRedLabel->hide();

	challengeRedLabel = new QLabel(selectionFrame);
	challengeRedLabel->resize(250, 250);
	challengeRedLabel->setScaledContents(true);
	challengeRedLabel->setPixmap(QPixmap(":/TaichiGame/Resources/challengeMode_red.png"));
	challengeRedLabel->move(648, 194);
	challengeRedLabel->hide();

	practiceGrayLabel = new QLabel(selectionFrame);
	practiceGrayLabel->resize(250, 250); 
	practiceGrayLabel->setScaledContents(true);
	practiceGrayLabel->setPixmap(QPixmap(":/TaichiGame/Resources/practiceMode_gray.png"));
	practiceGrayLabel->move(216, 194);

	challengeGrayLabel = new QLabel(selectionFrame);
	challengeGrayLabel->resize(250, 250);
	challengeGrayLabel->setScaledContents(true);
	challengeGrayLabel->setPixmap(QPixmap(":/TaichiGame/Resources/challengeMode_gray.png"));
	challengeGrayLabel->move(648, 194);

	practiceSelectedLabel = new QLabel(selectionFrame);
	practiceSelectedLabel->resize(250, 250);
	practiceSelectedLabel->setScaledContents(true);
	practiceSelectedLabel->setPixmap(QPixmap(":/TaichiGame/Resources/practiceModeSelected.png"));
	practiceSelectedLabel->move(216, 194);
	practiceSelectedLabel->hide();

	challengeSelectedLabel = new QLabel(selectionFrame);
	challengeSelectedLabel->resize(250, 250);
	challengeSelectedLabel->setScaledContents(true);
	challengeSelectedLabel->setPixmap(QPixmap(":/TaichiGame/Resources/challengeModeSelected.png"));
	challengeSelectedLabel->move(648, 194);
	challengeSelectedLabel->hide();

	demostrationWidget = new QVideoWidget(this);
	demostrationWidget->resize(1080,607);
	demoVideoPlayer.setVideoOutput(demostrationWidget);

	scoreWidget = new LabelWithBorder(this,1);
	scoreWidget->resize(1080, 607);

	thistimescoreHint = new QLabel(scoreWidget);
	thistimescoreHint->resize(150,45);
	thistimescoreHint->move(85,130);
	thistimescoreHint->setScaledContents(true);
	thistimescoreHint->setPixmap(QPixmap(":/TaichiGame/Resources/currentScore.png"));

	thisTimeScoreLabel = new QLabel(scoreWidget);
	thisTimeScoreLabel->resize(300, 100);
	thisTimeScoreLabel->move(250, 95);
	thisTimeScoreLabel->setAlignment(Qt::AlignCenter);
	thisTimeScoreLabel->setText("Score: ?");
	thisTimeScoreLabel->setStyleSheet("color: rgb(237,28,36);");
	thisTimeScoreLabel->setFont(QFont("Book Antiqua", 63, 18));

	leftFourWords = new QLabel(scoreWidget);
	leftFourWords->resize(155,300);
	leftFourWords->move(80,220);
	leftFourWords->setScaledContents(true);
	leftFourWords->setPixmap(QPixmap(":/TaichiGame/Resources/leftFourOfWords.png"));

	rightFourWords = new QLabel(scoreWidget);
	rightFourWords->resize(155,300);
	rightFourWords->move(400,220);
	rightFourWords->setScaledContents(true);
	rightFourWords->setPixmap(QPixmap(":/TaichiGame/Resources/rightFourOfWords.png"));

	juanhongScoreLabel = new QLabel(scoreWidget);
	juanhongScoreLabel->resize(208, 36);
	juanhongScoreLabel->move(250,225);
	juanhongScoreLabel->setAlignment(Qt::AlignLeft);
	juanhongScoreLabel->setText("0");
	juanhongScoreLabel->setFont(QFont("Book Antiqua", 22, 63, true));

	louxiScoreLabel = new QLabel(scoreWidget);
	louxiScoreLabel->resize(208, 36);
	louxiScoreLabel->move(250, 312);
	louxiScoreLabel->setAlignment(Qt::AlignLeft);
	louxiScoreLabel->setText("0");
	louxiScoreLabel->setFont(QFont("Book Antiqua", 22, 63, true));

	yemaScoreLabel = new QLabel(scoreWidget);
	yemaScoreLabel->resize(208, 36);
	yemaScoreLabel->move(250, 396);
	yemaScoreLabel->setAlignment(Qt::AlignLeft);
	yemaScoreLabel->setText("0");
	yemaScoreLabel->setFont(QFont("Book Antiqua", 22, 63, true));

	yunshouScoreLabel = new QLabel(scoreWidget);
	yunshouScoreLabel->resize(208, 36);
	yunshouScoreLabel->move(250, 484);
	yunshouScoreLabel->setAlignment(Qt::AlignLeft);
	yunshouScoreLabel->setText("0");
	yunshouScoreLabel->setFont(QFont("Book Antiqua", 22, 63, true));

	jinjiScoreLabel = new QLabel(scoreWidget);
	jinjiScoreLabel->resize(208, 36);
	jinjiScoreLabel->move(565, 225);
	jinjiScoreLabel->setAlignment(Qt::AlignLeft);
	jinjiScoreLabel->setText("0");
	jinjiScoreLabel->setFont(QFont("Book Antiqua", 22, 63, true));

	dengjiaoScoreLabel = new QLabel(scoreWidget);
	dengjiaoScoreLabel->resize(208, 36);
	dengjiaoScoreLabel->move(565, 312);
	dengjiaoScoreLabel->setAlignment(Qt::AlignLeft);
	dengjiaoScoreLabel->setText("0");
	dengjiaoScoreLabel->setFont(QFont("Book Antiqua", 22, 63, true));

	lanqueweiScoreLabel = new QLabel(scoreWidget);
	lanqueweiScoreLabel->resize(208, 36);
	lanqueweiScoreLabel->move(565, 396);
	lanqueweiScoreLabel->setAlignment(Qt::AlignLeft);
	lanqueweiScoreLabel->setText("0");
	lanqueweiScoreLabel->setFont(QFont("Book Antiqua", 22, 63, true));

	shizishouScoreLabel = new QLabel(scoreWidget);
	shizishouScoreLabel->resize(208, 36);
	shizishouScoreLabel->move(565, 484);
	shizishouScoreLabel->setAlignment(Qt::AlignLeft);
	shizishouScoreLabel->setText("0");
	shizishouScoreLabel->setFont(QFont("Book Antiqua", 22, 63, true));

	historyScoreHintLabel = new QLabel(scoreWidget);
	historyScoreHintLabel->resize(250,100);
	historyScoreHintLabel->move(760,30);
	historyScoreHintLabel->setScaledContents(true);
	historyScoreHintLabel->setPixmap(QPixmap(":/TaichiGame/Resources/historyListOfWords.png"));

	historyScoreModel = new QStandardItemModel(10, 2, scoreWidget);

	historyScoreview = new QTableView(scoreWidget);
	historyScoreview->setModel(historyScoreModel);
	historyScoreview->resize(300,442);
	historyScoreview->move(720,130);
	historyScoreview->setColumnWidth(0,188);
	historyScoreview->setColumnWidth(1,110);
	for (int i = 0; i < 10; i++)
	{
		historyScoreview->setRowHeight(i, 44);
	}
	historyScoreview->verticalHeader()->hide();
	historyScoreview->horizontalHeader()->hide();
	historyScoreview->setShowGrid(false);
	QFile historyScoreviewQSS(":/TaichiGame/Resources/historyScoreview.qss");
	historyScoreviewQSS.open(QFile::ReadOnly);
	historyScoreview->setStyleSheet(historyScoreviewQSS.readAll());
	historyScoreviewQSS.close();

	midStackedWidget = new QStackedWidget(this);
	midStackedWidget->resize(1080,607);
	midStackedWidget->addWidget(beginLabel);
	midStackedWidget->addWidget(selectionFrame);
	midStackedWidget->addWidget(demostrationWidget);
	midStackedWidget->addWidget(scoreWidget);
	midStackedWidget->move(0,288);

	beginMaskLabel = new QLabel(midStackedWidget);
	beginMaskLabel->resize(1080,607);
	//beginMaskLabel->setWindowFlags(Qt::WindowStaysOnTopHint);
	beginMaskLabel->setPixmap(QPixmap(":/TaichiGame/Resources/stackWidgetMask.png").scaledToWidth(1080));

	realTimeSceneLabel = new QLabel(this);
	realTimeSceneLabel->resize(1080, 607);
	realTimeSceneLabel->move(0, 950);

	realSceneMaskLabel = new QLabel(realTimeSceneLabel);
	realSceneMaskLabel->resize(1080, 607);
	realSceneMaskLabel->setPixmap(QPixmap(":/TaichiGame/Resources/humanDetection.png").scaledToWidth(1080));

	playBackWidget = new QVideoWidget(this);
	playBackWidget->resize(1080, 607);
	playBackWidget->move(0,950);
	playBackWidget->hide();

	//recLabel = new QLabel(this);
	//recLabel->resize(1080, 607);
	//recLabel->setWindowFlags(Qt::WindowStaysOnTopHint);
	//recLabel->move(0,288);
	//recLabel->setPixmap(QPixmap(":/TaichiGame/Resources/rec.png"));
	//recLabel->setScaledContents(true);

	playBackLabelIn = new QLabel(realSceneMaskLabel);
	playBackLabelIn->resize(100,200);
	playBackLabelIn->move(0,203);
	playBackLabelIn->setScaledContents(true);
	playBackLabelIn->setPixmap(QPixmap(":/TaichiGame/Resources/playBackIn.png"));
	playBackLabelIn->hide();

	playBackLabelOut = new QLabel(realSceneMaskLabel);
	playBackLabelOut->resize(200, 200);
	playBackLabelOut->move(0, 203);
	playBackLabelOut->setScaledContents(true);
	playBackLabelOut->setPixmap(QPixmap(":/TaichiGame/Resources/playBackOut.png"));
	playBackLabelOut->hide();

	returnLabelIn = new QLabel(realSceneMaskLabel);
	returnLabelIn->resize(100,200);
	returnLabelIn->move(980,203);
	returnLabelIn->setScaledContents(true);
	returnLabelIn->setPixmap(QPixmap(":/TaichiGame/Resources/returnBackIn.png"));
	returnLabelIn->hide();

	returnLabelOut = new QLabel(realSceneMaskLabel);
	returnLabelOut->resize(200,200);
	returnLabelOut->move(880,203);
	returnLabelOut->setScaledContents(true);
	returnLabelOut->setPixmap(QPixmap(":/TaichiGame/Resources/returnBackOut.png"));
	returnLabelOut->hide();

	playBackSelectedLabel = new QLabel(realSceneMaskLabel);
	playBackSelectedLabel->resize(200, 200);
	playBackSelectedLabel->move(0, 203);
	playBackSelectedLabel->setScaledContents(true);
	playBackSelectedLabel->setPixmap(QPixmap(":/TaichiGame/Resources/playBackSelected.png"));
	playBackSelectedLabel->hide();

	returnSelectedLabel = new QLabel(realSceneMaskLabel);
	returnSelectedLabel->resize(200, 200);
	returnSelectedLabel->move(880, 203);
	returnSelectedLabel->setScaledContents(true);
	returnSelectedLabel->setPixmap(QPixmap(":/TaichiGame/Resources/returnBackSelected.png"));
	returnSelectedLabel->hide();

	remindWidget = new LabelWithBorder(this, 0);
	remindWidget->resize(1080, 336);
	remindWidget->move(0, 1586);

	remindLabel = new QLabel(remindWidget);
	remindLabel->setAlignment(Qt::AlignCenter);
	remindLabel->setScaledContents(true);
	remindLabel->setPixmap(QPixmap(":/TaichiGame/Resources/standInTheBodyLine.png").scaledToWidth(800));
	remindLabel->resize(800, 110);
	remindLabel->move(140, 100);

	scoreBarLabel = new QLabel(remindWidget);
	scoreBarLabel->resize(1080, 130);
	scoreBarLabel->move(150,50);
	scoreBarLabel->hide();

	scoreArtworLabel = new QLabel(scoreBarLabel);
	scoreArtworLabel->resize(630,105);
	scoreArtworLabel->setScaledContents(true);
	scoreArtworLabel->setPixmap(QPixmap(":/TaichiGame/Resources/currentScoreArtword.png"));

	currentScoreProgBar = new QProgressBar(scoreBarLabel);
	currentScoreProgBar->move(195,15);
	currentScoreProgBar->setRange(30,100);
	currentScoreProgBar->setAlignment(Qt::AlignLeft);
	currentScoreProgBar->setValue(progBarTargetValue);
	currentScoreProgBar->setTextVisible(false);
	QFile currentScoreProgBarQSS(":/TaichiGame/Resources/currentScoreProgBar.qss");
	currentScoreProgBarQSS.open(QFile::ReadOnly);
	currentScoreProgBar->setStyleSheet(currentScoreProgBarQSS.readAll());
	currentScoreProgBarQSS.close();

	scoreSliderLabel = new QLabel(remindWidget);
	scoreSliderLabel->resize(1080, 130);
	scoreSliderLabel->move(150, 182);
	scoreSliderLabel->hide();

	rhythmArtworLabel = new QLabel(scoreSliderLabel);
	rhythmArtworLabel->resize(630, 105);
	rhythmArtworLabel->move(0,12);
	rhythmArtworLabel->setScaledContents(true);
	rhythmArtworLabel->setPixmap(QPixmap(":/TaichiGame/Resources/sliderArtword.png"));

	rhythmSlider = new QSlider(Qt::Horizontal,scoreSliderLabel);
	rhythmSlider->move(195, 0);
	rhythmSlider->setRange(-100,100);
	rhythmSlider->setValue(0);
	QFile rhythmSliderQSS(":/TaichiGame/Resources/rhythmSlider.qss");
	rhythmSliderQSS.open(QFile::ReadOnly);
	rhythmSlider->setStyleSheet(rhythmSliderQSS.readAll());
	rhythmSliderQSS.close();

	closeButton = new QPushButton(this);
	closeButton->setText("Close");
	closeButton->resize(100,30);
	closeButton->hide();

	readTimer = new QTimer(this);
	readTimer->start(10);
	 
	beginTimer = new QTimer(this);		//开始游戏时找到人后的等待开始时间

	scoreTimer = new QTimer(this);		//最终得分页面的停留时间

	sliderTimer = new QTimer(this);		//用于slider移动计时

	progBarTimer = new QTimer(this);	//用于progressBar移动计时
	
	releaseTimer = new QTimer(this);	//Scoring页后1秒释放videoWriter	
}
void TaichiGame::InitOthers()
{
	evaluateThread = new EvaluateThread(this);
	recordThread = new PlayBackRecordThread(this);
	gesture = new GestureDetection(0.05, 20, 5);
	m_pKinectSensor = NULL;
	m_pColorFrameReader = NULL;
	m_pColorRGBX = new RGBQUAD[cColorWidth * cColorHeight]; // create heap storage for color pixel data in RGBX format
	init();

	//背景音乐循环播放
	bgmList.addMedia(QUrl::fromLocalFile("./demoVideo/bgmAllWay.wma"));
	bgmList.setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
	bgmAllTheWay.setPlaylist(&bgmList);
	bgmAllTheWay.play();

	modeSelectionSound.setMedia(QUrl::fromLocalFile(":/TaichiGame/Resources/anniuSound.wma"));

	playBackVideo.setVideoOutput(playBackWidget);

	//历史分数的数据库
	historyScoreDataBase.createConnection();  //创建一个连接
	historyScoreDataBase.createTable();       //创建数据库表

	QFile file(":/TaichiGame/Resources/paraLengthTable.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "open failed!";
	}
	while (!file.atEnd())
	{
		QByteArray line = file.readLine();
		QString str(line);
		paraLengthTable.append(str.toInt());
	}
	paraLength = paraLengthTable[0];
	paraLenthShowed = paraLengthTable[0];
}
void TaichiGame::InitConnect()
{
	connect(readTimer, SIGNAL(timeout()), this, SLOT(OnReadFrame()));
	connect(beginTimer, SIGNAL(timeout()), this, SLOT(OnSwitch()));
	connect(sliderTimer, SIGNAL(timeout()), this, SLOT(OnMoveSlider())); 
	connect(progBarTimer, SIGNAL(timeout()), this, SLOT(OnMoveProgBar()));
	connect(releaseTimer, SIGNAL(timeout()), this, SLOT(OnReleaseVideoWriter()));
	connect(&demoVideoPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(OnSolveDemoMediaState(QMediaPlayer::MediaStatus)));
	connect(&demoVideoPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(OnMediaPositionChanged(qint64)));
	connect(&playBackVideo, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(OnSolvePlayBackMediaState(QMediaPlayer::MediaStatus)));
	connect(scoreTimer, SIGNAL(timeout()), this, SLOT(OnToTheVeryBeginning()));
	connect(this, SIGNAL(SigTransferData(int, QList<QList<int> >)), evaluateThread, SLOT(OnGetData(int, QList<QList<int> >)));
	qRegisterMetaType<EvaluateThread::MatchPosition>("EvaluateThread::MatchPosition");
	connect(evaluateThread, SIGNAL(SigCurrentScore(int, EvaluateThread::MatchPosition)), this, SLOT(OnUpdateCurrentScore(int, EvaluateThread::MatchPosition)));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}
void TaichiGame::OnSwitch()
{
	if (beginTimer->isActive())
	{
		beginTimer->stop(); 
		BeginTimerStarted = false;
	}

	int inidex = midStackedWidget->currentIndex()+1;
	midStackedWidget->setCurrentIndex(inidex);

	switch (midStackedWidget->currentIndex())
	{
	case 0:
		midLabelState = Beginning; 
		break;
	case 1:
		midLabelState = Selection;
		selectedMode = NoMode;
		realSceneMaskLabel->setPixmap(QPixmap(":/TaichiGame/Resources/normalMask.png").scaledToWidth(1080));
		remindLabel->setPixmap(QPixmap(":/TaichiGame/Resources/waveToSelect.png").scaledToWidth(800));
		remindLabel->resize(800,280);
		remindLabel->move(140,35);
		break;
	case 2:
		midLabelState = Demonstration;
		mediaPlayOrStop();
		ToStartRecordVideo = true;
		handCloseCounting.clear();
		remindLabel->setVisible(false);
		scoreBarLabel->setVisible(true);
		scoreSliderLabel->setVisible(true);
		currentScoreProgBar->setValue(30);
		rhythmSlider->setValue(0);
		//remindLabelForScore->setText(QString::fromLocal8Bit("当前得分：0 | 综合得分：0"));·········································
		break;
	case 3:
		midLabelState = Scoring;
		recordVideoStarted = false;
		isTaichiPlaying = false;
		dataFilterStarted = false;
		evaluateThread->timer->stop();
		frameNum = 0;
		mediaPlayOrStop();
		currentScoreProgBar->setValue(0);
		rhythmSlider->setValue(0);
		remindLabel->setVisible(true);
		scoreBarLabel->setVisible(false);
		scoreSliderLabel->setVisible(false);
		playBackLabelIn->show();
		returnLabelIn->show();
		beginMaskLabel->hide();
		remindLabel->setPixmap(QPixmap(":/TaichiGame/Resources/playbackOrReturn.png").scaledToWidth(800));
		paragraphIndex = 0;
		if (!scoreRemember.isEmpty())
		{
			writeDataBase(finalscoretoshow);
			readDataBaseAndShow();
			showScoreOfThisTime(finalscoretoshow, scoreRemember);
		}
		recordBodyData();
		scoreTimer->start(30000);      //得分界面持续5秒后，回到游戏初始状态
		releaseTimer->start(500);
		break;
	default:
		midLabelState = None;
		break;
	}
} 
void TaichiGame::OnSolveDemoMediaState(QMediaPlayer::MediaStatus status)
{
	if (midLabelState == Demonstration)
	{
		switch (status)
		{
		case QMediaPlayer::EndOfMedia:
			isTaichiPlaying = false;
			OnSwitch();
			break;
		default:
			break;
		}
	}
	else
		return;
}
void TaichiGame::OnSolvePlayBackMediaState(QMediaPlayer::MediaStatus status)
{
	switch (status)
	{
	case QMediaPlayer::EndOfMedia:
		isPlayingBack = false;
		midStackedWidget->setCurrentIndex(3);
		playBackWidget->hide();
		remindLabel->show();
		scoreBarLabel->hide();
		scoreSliderLabel->hide();
		realTimeSceneLabel->show();
		mediaPlayOrStop();
		cuurentPlayBackParaNo = 0;
		scoreTimer->start(30000);
		break;
	default:
		break;
	}
}
void TaichiGame::OnMediaPositionChanged(qint64 positon)
{//positionChanged(qint64)信号第一次发射是视频播放约761ms时，后每隔1000ms左右发射一次
	if (midLabelState == Demonstration)
	{
		if (positon > 13500)
		{
			isTaichiPlaying = true;
			evaluateThread->timer->start(12000);
		}
		else
		{
			return;
		}
	}

	if (isPlayingBack) //demo视频中起式的抬腿时间为14s
	{
		if (positon > 13750 + toMSeconds(paraLenthShowed))
		{
			if ((scoreRemember.count() < cuurentPlayBackParaNo + 1) || (rhythmRemember.count() < cuurentPlayBackParaNo + 1))
				return;
			OnUpdateCurrentScore(scoreRemember[cuurentPlayBackParaNo], rhythmRemember[cuurentPlayBackParaNo]);
			cuurentPlayBackParaNo++;
			paraLenthShowed += paraLengthTable[cuurentPlayBackParaNo];
		}
	}
}
void TaichiGame::OnUpdateCurrentScore(int score, EvaluateThread::MatchPosition pos) 
{
	if (score == 999)
	{
		QLabel *label = new QLabel(this);
		label->resize(400,400);
		label->move((this->width() - label->width()) / 2, (this->height() - label->height()) / 2);
		label->setStyleSheet("background-color: #ffffff;color:red;");
		label->setText(QString::fromLocal8Bit("由于您长时间未做动作\n游戏即将结束"));
		label->setFont(QFont("Microsoft YaHei",26,50));
		label->setWindowFlags(Qt::Tool|Qt::FramelessWindowHint);
		label->setAlignment(Qt::AlignCenter);
		label->show();
		label->setAttribute(Qt::WA_DeleteOnClose);

		QTimer *returnHintTimer = new QTimer;
		returnHintTimer->setSingleShot(true);
		returnHintTimer->start(3000);
		connect(returnHintTimer, SIGNAL(timeout()), label, SLOT(close()));
		connect(returnHintTimer, SIGNAL(timeout()), this, SLOT(OnToTheVeryBeginning()));
		connect(returnHintTimer, SIGNAL(timeout()), evaluateThread, SLOT(OnTerminate()));
		return;
	}
		
	switch (pos)
	{
	case EvaluateThread::NonePositon:
		break;
	case EvaluateThread::LeftTwo:
		if (score > 70)
			targetPosForSlider = -66;
		else
			targetPosForSlider = -100;
		break;
	case EvaluateThread::LeftOne:
		if (score > 70)
			targetPosForSlider = -33;
		else
			targetPosForSlider = -50;
		break;
	case EvaluateThread::Center:
		targetPosForSlider = 0;
		break;
	case EvaluateThread::RightOne:
		if (score > 70)
			targetPosForSlider = 33;
		else
			targetPosForSlider = 50;
		break;
	case EvaluateThread::RightTwo:
		if (score > 70)
			targetPosForSlider = 66;
		else
			targetPosForSlider = 100;
		break;
	}
	if (rhythmSlider->value() != targetPosForSlider)
	{
		sliderTimer->start(2);
	}

	//if (55 >score)
	//	currentScore = LessThan55;
	//else if (score >= 55 && 65 > score)
	//	currentScore = From55To65;
	//else if (score >= 65 && 75 > score)
	//	currentScore = From65To75;
	//else if (score >= 75 && 85 > score)
	//	currentScore = From75To85;
	//else if (score >= 85 && 95 > score)
	//	currentScore = From85To95;
	//else
	//	currentScore = MoreThan95;
	//

	progBarTargetValue = score;

	if (currentScoreProgBar->value() != progBarTargetValue)
	{
		progBarTimer->start(5);
	}
	if (!isPlayingBack)
	{
		scoreRemember.append(score);
		rhythmRemember.append(pos);

		int finalScore = 0;
		for (int i = 0; i < scoreRemember.count(); i++)
		{
			finalScore += scoreRemember[i];
		}
		finalScore = finalScore / scoreRemember.count();

		//remindLabelForScore->setText(QString::fromLocal8Bit("当前得分：") +
		//QString::number(score) + ' ' + '|' + ' ' + QString::fromLocal8Bit("综合得分：") + 
		//QString::number(finalScore));

		finalscoretoshow = finalScore;
	}
}
void TaichiGame::OnMoveSlider()
{
	if (rhythmSlider->value() < targetPosForSlider)
	{
		rhythmSlider->setValue(rhythmSlider->value() + 1);
	}
	else if (rhythmSlider->value() > targetPosForSlider)
	{
		rhythmSlider->setValue(rhythmSlider->value() - 1);
	}
	else
	{
		sliderTimer->stop();
	}
}
void TaichiGame::OnMoveProgBar()
{
	if (currentScoreProgBar->value() < progBarTargetValue)
	{
		currentScoreProgBar->setValue(currentScoreProgBar->value() + 1);
	}
	else if (currentScoreProgBar->value() > progBarTargetValue)
	{
		currentScoreProgBar->setValue(currentScoreProgBar->value() - 1);
	}
	else
	{
		progBarTimer->stop();
	}
}
void TaichiGame::OnToTheVeryBeginning()
{
	scoreTimer->stop();
	midStackedWidget->setCurrentIndex(0);
	remindLabel->setVisible(true);
	scoreBarLabel->setVisible(false);
	scoreSliderLabel->setVisible(false);
	realSceneMaskLabel->setPixmap(QPixmap(":/TaichiGame/Resources/humanDetection.png").scaledToWidth(1080));
	remindLabel->setPixmap(QPixmap(":/TaichiGame/Resources/standInTheBodyLine.png").scaledToWidth(800));
	remindLabel->move(140, 100);
	remindLabel->resize(800, 110);
	midLabelState = Beginning;
	isTaichiPlaying = false;
	playBackLabelIn->hide();
	playBackLabelOut->hide();
	returnLabelIn->hide();
	returnLabelOut->hide();
	playBackSelectedLabel->hide();
	returnSelectedLabel->hide();
	beginMaskLabel->show();
	mediaPlayOrStop();
	scoreRemember.clear();
	rhythmRemember.clear();
	paraLength = paraLengthTable[0];
	paraLenthShowed = paraLengthTable[0];
	cuurentPlayBackParaNo = 0;
	currentScoreProgBar->setValue(0);
	rhythmSlider->setValue(0);
	frameNum = 0;
	evaluateThread->timer->stop();
	dataFilterStarted = false;
	recordVideoStarted = false;
	releaseTimer->start(500);
}
void TaichiGame::OnReadFrame()
{
	updateColorData();
	//updateDepthData();
	updateBodyData();
}
void TaichiGame::OnReleaseVideoWriter()
{
	recordThread->releaseVideoWriter();
	releaseTimer->stop();
}
void TaichiGame::mouseReleaseEvent(QMouseEvent *ev)
{
	if (ev->button() == Qt::RightButton && ev->pos().y() < 216)
	{
		closeButton->setVisible(true);
		closeButton->move(ev->pos());
	}
	QWidget::mouseReleaseEvent(ev);
}
void TaichiGame::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		closeButton->setVisible(false);
	}
	QWidget::mousePressEvent(event);
}
void TaichiGame::paintEvent(QPaintEvent *ev)
{
	QPainter pp(this);
	QRect Temp(0, 0, this->width(), this->height());
	pp.drawImage(Temp, backImg);
}
void TaichiGame::updateColorData()
{
	if (!m_pColorFrameReader)
	{
		return;
	} 
	IColorFrame* pColorFrame = NULL;

	HRESULT hr = m_pColorFrameReader->AcquireLatestFrame(&pColorFrame);

	if (SUCCEEDED(hr))
	{
		IFrameDescription* pFrameDescription = NULL;
		int nWidth = 0;
		int nHeight = 0;
		ColorImageFormat imageFormat = ColorImageFormat_None;
		UINT nBufferSize = 0;
		RGBQUAD *pBuffer = NULL;

		if (SUCCEEDED(hr))
		{
			hr = pColorFrame->get_FrameDescription(&pFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFrameDescription->get_Width(&nWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFrameDescription->get_Height(&nHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
		}

		if (SUCCEEDED(hr))
		{
			if (imageFormat == ColorImageFormat_Bgra)
			{
				hr = pColorFrame->AccessRawUnderlyingBuffer(&nBufferSize, reinterpret_cast<BYTE**>(&pBuffer));
			}
			else if (m_pColorRGBX) 
			{
				pBuffer = m_pColorRGBX;
				nBufferSize = cColorWidth * cColorHeight * sizeof(RGBQUAD);
				hr = pColorFrame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(pBuffer), ColorImageFormat_Bgra);
			}
			else
			{
				hr = E_FAIL;
			}
		}
		if (SUCCEEDED(hr))
		{
			if (ToStartRecordVideo)
			{
				recordThread->openVideoFile();
				ToStartRecordVideo = false;
				recordVideoStarted = true;
			}
			if (recordVideoStarted)
			{
				recordThread->OnRecieveFrameData(reinterpret_cast<BYTE*>(pBuffer));
			}
			realTimeSceneLabel->setPixmap(QPixmap::fromImage(QImage(reinterpret_cast<uchar*>(pBuffer), nWidth, nHeight, QImage::Format_RGB32)).scaledToWidth(1080));
			//qDebug() << QDateTime::currentMSecsSinceEpoch() << "colorframe";
		}

		SafeRelease(pFrameDescription);
	}

	SafeRelease(pColorFrame);
	return;
}
void TaichiGame::updateDepthData()
{
	//if (!m_pDepthFrameReader)
	//{
	//	return;
	//}
	//IDepthFrame* pDepthFrame = NULL;

	//HRESULT hr = m_pDepthFrameReader->AcquireLatestFrame(&pDepthFrame);

	//if (SUCCEEDED(hr))
	//{
	//	//INT64 nTime = 0;
	//	IFrameDescription* pFrameDescription = NULL;
	//	int nWidth = 0;
	//	int nHeight = 0;
	//	USHORT nDepthMinReliableDistance = 0;
	//	USHORT nDepthMaxDistance = 0;
	//	UINT nBufferSize = 0;
	//	UINT16 *pBuffer = NULL;

	//	//hr = pDepthFrame->get_RelativeTime(&nTime);

	//	if (SUCCEEDED(hr))
	//	{
	//		hr = pDepthFrame->get_FrameDescription(&pFrameDescription);
	//	}

	//	if (SUCCEEDED(hr))
	//	{
	//		hr = pFrameDescription->get_Width(&nWidth);
	//	}

	//	if (SUCCEEDED(hr))
	//	{
	//		hr = pFrameDescription->get_Height(&nHeight);
	//	}

	//	if (SUCCEEDED(hr))
	//	{
	//		hr = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
	//	}

	//	if (SUCCEEDED(hr))
	//	{
	//		// In order to see the full range of depth (including the less reliable far field depth)
	//		// we are setting nDepthMaxDistance to the extreme potential depth threshold
	//		nDepthMaxDistance = USHRT_MAX;

	//		// Note:  If you wish to filter by reliable depth distance, uncomment the following line.
	//		//// hr = pDepthFrame->get_DepthMaxReliableDistance(&nDepthMaxDistance);
	//	}

	//	if (SUCCEEDED(hr))
	//	{
	//		hr = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
	//	}

	//	if (SUCCEEDED(hr))
	//	{
	//		//ui.depthLabel->setPixmap(QPixmap::fromImage(QImage(reinterpret_cast<BYTE*>(pBuffer), nWidth, nHeight, 512 * 4, QImage::Format_ARGB32_Premultiplied)).scaledToWidth(340));
	//		//ProcessDepth(nTime, pBuffer, nWidth, nHeight, nDepthMinReliableDistance, nDepthMaxDistance);
	//	}

	//	SafeRelease(pFrameDescription);
	//}

	//SafeRelease(pDepthFrame);
}
void TaichiGame::updateBodyData()
{
	Mat skeletonImage;
	skeletonImage.create(cDepthHeight, cDepthWidth, CV_8UC3);
	skeletonImage.setTo(0);
	CvPoint skeletonPoint[BODY_COUNT][JointType_Count] = { cvPoint(0, 0) };
	int depthValue[BODY_COUNT][JointType_Count] = {0};
	if (!m_pBodyFrameReader)
	{
		return;
	}
	IBodyFrame* pBodyFrame = NULL;
	HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

	if (SUCCEEDED(hr))
	{
		IBody* ppBodies[BODY_COUNT] = { 0 };
		if (SUCCEEDED(hr))
		{
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}
		if (SUCCEEDED(hr))
		{
			for (int i = 0; i < BODY_COUNT; ++i) //一般情形下的读数据
			{
				IBody* pBody = ppBodies[i];
				if (pBody)
				{
					BOOLEAN bTracked = false;
					hr = pBody->get_IsTracked(&bTracked);
					if (SUCCEEDED(hr) && bTracked)
					{
						qDebug() << QDateTime::currentMSecsSinceEpoch() << "bodytimetest";
						HandState leftHandState = HandState_Unknown;
						HandState rightHandState = HandState_Unknown;

						pBody->get_HandLeftState(&leftHandState);
						pBody->get_HandRightState(&rightHandState);

						Joint joints[JointType_Count];
						hr = pBody->GetJoints(_countof(joints), joints);
						//画关节
						if (SUCCEEDED(hr))
						{
							for (int j = 0; j < _countof(joints); ++j)
							{
								DepthSpacePoint depthPoint = { 0 };
								m_pCoordinateMapper->MapCameraPointToDepthSpace(joints[j].Position, &depthPoint);
								skeletonPoint[i][j].x = static_cast<int>(depthPoint.X);
								skeletonPoint[i][j].y = static_cast<int>(depthPoint.Y);
								depthValue[i][j] = joints[j].Position.Z * 100;
								circle(skeletonImage, skeletonPoint[i][j], 3/*半径*/, cvScalar(255, 255, 255), 1, 8, 0);
								//画关节点位置，保存关节点数据
							}
						}

						//识别到人体开始演练
						int headDepth = joints[3].Position.Z * 100;//头部的深度值
						if (midLabelState == Beginning && !BeginTimerStarted && headDepth>180 && headDepth<230)
						{
							beginTimer->start(3000);
							BeginTimerStarted = true;
							remindLabel->setPixmap(QPixmap(":/TaichiGame/Resources/stayStaticly.png").scaledToWidth(800));
						}

						//练习或挑战模式选择
						if ((headDepth < 180 || headDepth > 230) && midLabelState == Selection)
							continue;
						if (SUCCEEDED(hr) && midLabelState == Selection)
						{
							pBody->get_HandLeftState(&leftHandStateForDetection);
							pBody->get_HandRightState(&rightHandStateForDetection);
							gesture->Update(pBody);
							if (gesture->wave_datas[0][1].Pos == Left && gesture->IsSkeletonTrackedWell(pBody, 1))
							{
								if (handPosLastState != HandUpLeft)
								{
									modeSelectionSound.play();
									handPosLastState = HandUpLeft;
								}
								if (!(rightHandStateForDetection == HandState_Closed))
								{
									practiceRedLabel->show();
									challengeRedLabel->hide();
									practiceGrayLabel->hide();
									challengeGrayLabel->show();
									practiceSelectedLabel->hide();
									challengeSelectedLabel->hide();
									if (coutingBegan)
										handCloseCounting.append(0);
								}
								else
								{
									if (!coutingBegan)
										coutingBegan = true;
									handCloseCounting.append(1);
									practiceSelectedLabel->show();
									challengeSelectedLabel->hide();
								}
							}
							else if (gesture->wave_datas[0][1].Pos == Right && gesture->IsSkeletonTrackedWell(pBody, 1))
							{
								if (handPosLastState != HandUpRight)
								{
									modeSelectionSound.play();
									handPosLastState = HandUpRight;
								}
								if (!(rightHandStateForDetection == HandState_Closed))
								{
									practiceRedLabel->hide();
									challengeRedLabel->show();
									practiceGrayLabel->show();
									challengeGrayLabel->hide();
									practiceSelectedLabel->hide();
									challengeSelectedLabel->hide();
									if (coutingBegan)
										handCloseCounting.append(0);
								}
								else
								{
									if (!coutingBegan)
										coutingBegan = true;
									handCloseCounting.append(2);
									practiceSelectedLabel->hide();
									challengeSelectedLabel->show();
								}
							}
							else
							{
								practiceRedLabel->hide();
								challengeRedLabel->hide();
								practiceGrayLabel->show();
								challengeGrayLabel->show();
								practiceSelectedLabel->hide();
								challengeSelectedLabel->hide();
								handPosLastState = HandDown;
								coutingBegan = false;
								handCloseCounting.clear();
							}
							if (handCloseCounting.count() == 20)
							{
								int one_left_counting = handCloseCounting.count(1);
								int two_right_counting = handCloseCounting.count(2);
								if ((one_left_counting + two_right_counting) > handCloseCounting.count() / 2)
								{
									if (one_left_counting > two_right_counting)
										selectedMode = WihtDemonstration_Left;
									else
										selectedMode = WihtoutDemonstration_Right;

									handCloseCounting.clear();
									coutingBegan = false;
									handPosLastState = HandDown;
									practiceRedLabel->hide();
									challengeRedLabel->hide();
									practiceGrayLabel->show();
									challengeGrayLabel->show();
									practiceSelectedLabel->hide();
									challengeSelectedLabel->hide();
									OnSwitch();
								}
								else
								{
									handCloseCounting.clear();
									coutingBegan = false;
									handPosLastState = HandDown;
								}
							}
						}//模式选择

						//记录并向评分线程发送骨骼数据
						qreal curBodyDis = qSqrt(qPow(skeletonPoint[i][3].x, 2) +
							qPow(skeletonPoint[i][3].y, 2) +
							qPow(joints[3].Position.Z * 100, 2));
						if ((qAbs(lastFrameDis - curBodyDis) > 200) && (midLabelState == Demonstration) && dataFilterStarted)
						{
							continue;
						}
						if (SUCCEEDED(hr) && isTaichiPlaying && paragraphIndex < 30)
						{
							frameNum++;
							QList<int> temprow;
							for (int j = 0; j < _countof(joints); ++j)
							{
								DepthSpacePoint depthPoint = { 0 };
								m_pCoordinateMapper->MapCameraPointToDepthSpace(joints[j].Position, &depthPoint);
								skeletonPoint[i][j].x = static_cast<int>(depthPoint.X);
								skeletonPoint[i][j].y = static_cast<int>(depthPoint.Y);
								temprow.append(skeletonPoint[i][j].x);
								temprow.append(skeletonPoint[i][j].y);
								temprow.append(joints[j].Position.Z * 100);
							}
							lastFrameDis = qSqrt(qPow(skeletonPoint[i][3].x, 2) +
								qPow(skeletonPoint[i][3].y, 2) +
								joints[3].Position.Z * 100);
							dataFilterStarted = true;

							bodyDataToRecord.append(temprow);
							if (paragraphsData.count() < paraLength)
							{
								if ((frameNum % 10) != 0 && !temprow.isEmpty())
								{
									paragraphsData.append(temprow);
								}
							}
							else
							{
								tailDataRecording = true;
								lastParagraphsData.clear();
								lastParagraphsData.append(paragraphsData);
								//emit SigTransferData(paragraphIndex, paragraphsData); 
								paragraphIndex++;
								paraLength = paraLengthTable[paragraphIndex];
								paragraphsData.clear();
								paragraphsData.append(temprow);
							}
							if (tailDataRecording)
							{
								tailData.append(temprow);
								if (tailData.count() == 30)
								{
									tailDataRecording = false;
									QList<QList<int>> emitData;
									emitData.append(headData);
									emitData.append(lastParagraphsData);
									emitData.append(tailData);
									emit SigTransferData(paragraphIndex - 1, emitData);
									tailData.clear();
									headData.clear();
									headData.append(lastParagraphsData.mid(lastParagraphsData.count() - 31, 30));
								}
							}
						}

						//回放或返回选择
						if (SUCCEEDED(hr) && (midLabelState == Scoring) && !isPlayingBack)
						{
							pBody->get_HandLeftState(&leftHandStateForDetection);
							pBody->get_HandRightState(&rightHandStateForDetection);
							double Dis_HipLeftToLeftHand = qSqrt(qPow(skeletonPoint[i][7].x - skeletonPoint[i][12].x, 2) +
								qPow(skeletonPoint[i][7].y - skeletonPoint[i][12].y, 2) + 
								qPow(depthValue[i][7] - depthValue[i][12], 2));
							double Dis_HipRightToRightHand = qSqrt(qPow(skeletonPoint[i][11].x - skeletonPoint[i][16].x, 2) +
								qPow(skeletonPoint[i][11].y - skeletonPoint[i][16].y, 2) +
								qPow(depthValue[i][11] - depthValue[i][16], 2));
							double Threashold = qSqrt(qPow(skeletonPoint[i][0].x - skeletonPoint[i][20].x, 2) +
								qPow(skeletonPoint[i][0].y - skeletonPoint[i][20].y, 2) +
								qPow(depthValue[i][0] - depthValue[i][20], 2));;
							if (Dis_HipLeftToLeftHand > Threashold)
							{
								if (handPosLastState != HandUpLeft)
								{
									modeSelectionSound.play();
									handPosLastState = HandUpLeft;
								}
								if (!(leftHandStateForDetection == HandState_Closed))
								{
									playBackLabelIn->hide();
									playBackLabelOut->show();
									returnLabelIn->show();
									returnLabelOut->hide();
									playBackSelectedLabel->hide();
									returnSelectedLabel->hide();
									if (coutingBegan)
										handCloseCounting.append(0);
								}
								else
								{
									if (!coutingBegan)
										coutingBegan = true;
									handCloseCounting.append(1);
									playBackLabelIn->hide();
									playBackLabelOut->hide();
									returnLabelIn->hide();
									returnLabelOut->hide();
									playBackSelectedLabel->show();
									returnSelectedLabel->hide();
								}
							}
							else if (Dis_HipRightToRightHand > Threashold)
							{
								if (handPosLastState != HandUpRight)
								{
									modeSelectionSound.play();
									handPosLastState = HandUpRight;
								}
								if (!(rightHandStateForDetection == HandState_Closed))
								{
									playBackLabelIn->show();
									playBackLabelOut->hide();
									returnLabelIn->hide();
									returnLabelOut->show();
									playBackSelectedLabel->hide();
									returnSelectedLabel->hide();
									if (coutingBegan)
										handCloseCounting.append(0);
								}
								else
								{
									if (!coutingBegan)
										coutingBegan = true;
									handCloseCounting.append(2);
									playBackLabelIn->hide();
									playBackLabelOut->hide();
									returnLabelIn->hide();
									returnLabelOut->hide();
									playBackSelectedLabel->hide();
									returnSelectedLabel->show();
								}
							}
							else
							{
								playBackLabelIn->show();
								playBackLabelOut->hide();
								returnLabelIn->show();
								returnLabelOut->hide();
								playBackSelectedLabel->hide();
								returnSelectedLabel->hide();
								handPosLastState = HandDown;
								coutingBegan = false;
								handCloseCounting.clear();
							}
							if (handCloseCounting.count() == 30)
							{
								int one_left_counting = handCloseCounting.count(1);
								int two_right_counting = handCloseCounting.count(2);
								if ((one_left_counting + two_right_counting) > handCloseCounting.count() / 2)
								{
									if (one_left_counting > two_right_counting)
										playBack();
									else
										OnToTheVeryBeginning();

									playBackLabelIn->hide();
									playBackLabelOut->hide();
									returnLabelIn->hide();
									returnLabelOut->hide();
									playBackSelectedLabel->hide();
									returnSelectedLabel->hide();

									handPosLastState = HandDown;
									coutingBegan = false;
									handCloseCounting.clear();
								}
								else
								{
									handPosLastState = HandDown;
									coutingBegan = false;
									handCloseCounting.clear();
								}
							}
							if (handCloseCounting.count() > 30)
								handCloseCounting.clear();
						}//回放或返回选择
					}
				}
			}
			tempLabel->setPixmap(QPixmap::fromImage(cvMat2QImage(skeletonImage)).scaledToWidth(500));
				//ProcessBody(nTime, BODY_COUNT, ppBodies);
		}
		for (int i = 0; i < _countof(ppBodies); ++i)  
		{
			SafeRelease(ppBodies[i]);
		}
	}
	SafeRelease(pBodyFrame);
}
HRESULT TaichiGame::init()
{
	HRESULT hr;

	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr))
	{
		return -1;
	}
	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get the c/d/b reader
		IColorFrameSource* pColorFrameSource = NULL;
		IDepthFrameSource* pDepthFrameSource = NULL;
		IBodyFrameSource* pBodyFrameSource = NULL;
		hr = m_pKinectSensor->Open();
		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}
		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_ColorFrameSource(&pColorFrameSource);
		}
		if (SUCCEEDED(hr))
		{
			hr = pColorFrameSource->OpenReader(&m_pColorFrameReader);
		}
		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
		}
		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameSource->OpenReader(&m_pDepthFrameReader);
		}
		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
		}
		if (SUCCEEDED(hr))
		{
			hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
		}
		SafeRelease(pBodyFrameSource);
		SafeRelease(pColorFrameSource);
		SafeRelease(pDepthFrameSource);
	}

	if (!m_pKinectSensor || FAILED(hr))
	{
		qDebug() << "No ready Kinect found! \n";
		return -1;
	}
	return hr;
}
void TaichiGame::writeDataBase(int score)
{
	//得分写入数据库并排序
	QDateTime currentTime = QDateTime::currentDateTime();
	QString currentTimeStr = QString(currentTime.toString("yyyy") + '-'
		+ currentTime.toString("MM") + '-' + currentTime.toString("dd")
		+ ' ' + currentTime.toString("hh") + ':' + currentTime.toString("mm"));
	historyScoreDataBase.insert(QDateTime::currentMSecsSinceEpoch(), currentTimeStr, score);
	historyScoreDataBase.sortByScore();
}
void TaichiGame::readDataBaseAndShow()
{
	QStringList dates = historyScoreDataBase.queryAllDate();
	for (int i = 0; i < dates.count(); i++)
	{
		historyScoreModel->setItem(i, 0,new QStandardItem(dates[i]));
	}
	QList<int> scores = historyScoreDataBase.queryAllScore();
	for (int i = 0; i < scores.count(); i++)
	{
		historyScoreModel->setItem(i, 1, new QStandardItem(QString::number(scores[i])));
	}

	int numToShow;
	if (scores.count() >10)
		numToShow = 10;
	else
		numToShow = scores.count();

	for (int i = 0; i < numToShow; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			historyScoreModel->item(i, j)->setFont(QFont("Book Antiqua", 15, QFont::Bold));
			historyScoreModel->item(i, j)->setTextAlignment(Qt::AlignCenter);
		}
	}
	historyScoreModel->sort(1, Qt::DescendingOrder);
}
void TaichiGame::showScoreOfThisTime(int score, QList<int> scoreRem)
{
	//得分显示与结果动画
	thisTimeScoreLabel->setText(QString::number(score));
	if (scoreRem.count() == 30)
	{
		if (scoreRem[0] > scoreRem[29])
		{
			scoreRem.pop_back();
		}
		else
		{
			scoreRem.pop_front();
		}
		showRespScore(scoreRem);
	}
	else
	{
		showRespScore(scoreRem);
	}
	
	/*if (score >= 80)
		remindLabel->setText(QString::fromLocal8Bit("真棒！"));
	else if (80 >= score && score >= 60)
		remindLabel->setText(QString::fromLocal8Bit("不错！"));
	else
		remindLabel->setText(QString::fromLocal8Bit("再接再厉！"));*/
}
void TaichiGame::mediaPlayOrStop()
{
	if (demostrationWidget->isVisible())
	{
		QString fileName;
		if (selectedMode == WihtoutDemonstration_Right)
			fileName = QString("./demoVideo/taichiWithouthint.MP4");
		else
			fileName = QString("./demoVideo/taichiWithhint.MP4");
		demoVideoPlayer.setMedia(QUrl::fromLocalFile(fileName));
		bgmAllTheWay.pause();
		demoVideoPlayer.play();
	}
	else
	{
		demoVideoPlayer.stop();
		demoVideoPlayer.setMedia(NULL);
		bgmAllTheWay.play();
	}
}
void TaichiGame::playBack()
{
	isPlayingBack = true;
	playBackWidget->show();
	playBackVideo.setMedia(QUrl::fromLocalFile("./PlayBack/playbackVideo.avi"));
	remindLabel->hide();
	scoreBarLabel->show();
	scoreSliderLabel->show();
	realTimeSceneLabel->hide();
	playBackVideo.play();
	midStackedWidget->setCurrentIndex(2);
	mediaPlayOrStop();
	scoreTimer->stop();
}
void TaichiGame::recordBodyData()
{
	QDateTime currentTime = QDateTime::currentDateTime();
	QString currentTimeStr = QString(currentTime.toString("yyyy") + '-'
		+ currentTime.toString("MM") + '-' + currentTime.toString("dd")
		+ ' ' + currentTime.toString("hh") + '_' + currentTime.toString("mm"));
	QFile file(QString("./HistoryBodyData/%1.txt").arg(currentTimeStr));
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
	{
		qDebug() << "Open Failed!";
	}
	QTextStream in(&file);
	for (int i = 0; i < bodyDataToRecord.count(); i++)
	{
		for (int j = 0; j < bodyDataToRecord[i].count(); j++)
		{
			in << bodyDataToRecord[i][j] << ' ';
		}
		in << '\n';
	}
	file.close();
	bodyDataToRecord.clear();
}
int TaichiGame::toMSeconds(int paraLenth)
{
	return static_cast<int>((paraLenth / 30) * 1000);
}
void TaichiGame::showRespScore(QList<int> scoreRem)
{
	int sunParaScore = 0;

	for (int i = 3; i < 7; i++)
		sunParaScore += scoreRem[i];
	sunParaScore = sunParaScore / 4;
	juanhongScoreLabel->setText(QString::number(sunParaScore));
	sunParaScore = 0;

	for (int i = 7; i < 9; i++)
		sunParaScore += scoreRem[i];
	sunParaScore = sunParaScore / 2;
	louxiScoreLabel->setText(QString::number(sunParaScore));
	sunParaScore = 0;

	for (int i = 9; i < 11; i++)
		sunParaScore += scoreRem[i];
	sunParaScore = sunParaScore / 2;
	yemaScoreLabel->setText(QString::number(sunParaScore));
	sunParaScore = 0;

	for (int i = 11; i < 15; i++)
		sunParaScore += scoreRem[i];
	sunParaScore = sunParaScore / 4;
	yunshouScoreLabel->setText(QString::number(sunParaScore));
	sunParaScore = 0;

	for (int i = 15; i < 17; i++)
		sunParaScore += scoreRem[i];
	sunParaScore = sunParaScore / 2;
	jinjiScoreLabel->setText(QString::number(sunParaScore));
	sunParaScore = 0;

	for (int i = 17; i < 19; i++)
		sunParaScore += scoreRem[i];
	sunParaScore = sunParaScore / 2;
	dengjiaoScoreLabel->setText(QString::number(sunParaScore));
	sunParaScore = 0;

	for (int i = 19; i < 25; i++)
		sunParaScore += scoreRem[i];
	sunParaScore = sunParaScore / 6;
	lanqueweiScoreLabel->setText(QString::number(sunParaScore));
	sunParaScore = 0;

	if (scoreRem.count() == 29)
	{
		for (int i = 25; i < 28; i++)
			sunParaScore += scoreRem[i];
		sunParaScore = sunParaScore / 3;
		qDebug() << sunParaScore << scoreRem.count() << "2018_4_12";
		shizishouScoreLabel->setText(QString::number(sunParaScore));
		sunParaScore = 0;
	}
	else if ((scoreRem.count() < 29) && (scoreRem.count() >= 25))
	{
		for (int i = 25; i < scoreRem.count(); i++)
			sunParaScore += scoreRem[i];
		sunParaScore = sunParaScore / (scoreRem.count() - 25);
		qDebug() << sunParaScore << scoreRem.count() << "2018_4_12";
		shizishouScoreLabel->setText(QString::number(sunParaScore));
		sunParaScore = 0;
	}
	else
	{
		qDebug() << sunParaScore << scoreRem.count() << "2018_4_12";
		return;
	}
}
QImage TaichiGame::cvMat2QImage(const Mat &mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)  
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat
		pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3  
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat  
		pSrc = (uchar*)mat.data;
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		qDebug() << "CV_8UC4";
		// Copy input Mat  
		pSrc = (uchar*)mat.data;
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		qDebug() << "ERROR: Mat could not be converted to QImage.";
		return QImage();
	}
}