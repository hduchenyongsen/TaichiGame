#include "registerwidget.h"
#include <QStyleOption>
#include <QPainter>
#include <QLayout>
#include <windows.h>
#include <QFile>
#include <QTextStream>
#include <QUuid>
#include <QCryptographicHash>

RegisterWidget::RegisterWidget(QWidget *parent)
	: QWidget(parent)
{
	initUi(); 
	initConect();
}

RegisterWidget::~RegisterWidget()
{

}
void RegisterWidget::paintEvent(QPaintEvent *ev)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(ev);
}
void RegisterWidget::initUi()
{
	this->setFixedSize(720, 360);
	this->setStyleSheet("background-color: rgb(235,235,235);");

	editorOne = new QLineEdit(this);
	editorOne->setFixedWidth(50);
	editorOne->setAlignment(Qt::AlignCenter);

	editorTwo = new QLineEdit(this);
	editorTwo->setFixedWidth(50);
	editorTwo->setAlignment(Qt::AlignCenter);

	editorThree = new QLineEdit(this);
	editorThree->setFixedWidth(50);
	editorThree->setAlignment(Qt::AlignCenter);

	editorFour = new QLineEdit(this);
	editorFour->setFixedWidth(50);
	editorFour->setAlignment(Qt::AlignCenter);

	regCodeLabel = new QLabel(this);
	regCodeLabel->setText(QString::fromLocal8Bit("注册码："));

	line1 = new QLabel(this);
	line1->setText("-");

	line2 = new QLabel(this);
	line2->setText("-");

	line3 = new QLabel(this);
	line3->setText("-");

	regResult = new QLabel(this);
	//regResult->hide();

	registerButton = new QPushButton(this);
	registerButton->setText(QString::fromLocal8Bit("注册激活"));
	registerButton->setEnabled(false);

	nextButton = new QPushButton(this);
	nextButton->setText(QString::fromLocal8Bit("下一步"));
	nextButton->setEnabled(false);

	quitButton = new QPushButton(this);
	quitButton->setText(QString::fromLocal8Bit("取消"));

	QBoxLayout *editorLayout = new QHBoxLayout;
	editorLayout->addStretch();
	editorLayout->addWidget(regCodeLabel);
	editorLayout->addWidget(editorOne);
	editorLayout->addWidget(line1);
	editorLayout->addWidget(editorTwo);	
	editorLayout->addWidget(line2);
	editorLayout->addWidget(editorThree);
	editorLayout->addWidget(line3);
	editorLayout->addWidget(editorFour);
	editorLayout->addStretch();

	QBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch();
	buttonLayout->addWidget(registerButton);
	buttonLayout->addWidget(nextButton);
	buttonLayout->addWidget(quitButton);
	buttonLayout->addSpacing(20);

	QBoxLayout *resultWordLayout = new QHBoxLayout;
	resultWordLayout->addStretch(); 
	resultWordLayout->addWidget(regResult);
	resultWordLayout->addSpacing(20);

	QBoxLayout *layout = new QVBoxLayout;
	layout->addStretch();
	layout->addLayout(editorLayout);
	layout->addSpacing(100);
	layout->addLayout(buttonLayout);
	layout->addSpacing(5);
	layout->addLayout(resultWordLayout);
	layout->addSpacing(30);

	this->setLayout(layout);
}
void RegisterWidget::initConect()
{
	connect(editorOne, SIGNAL(textEdited(QString)), this, SLOT(OneToNext(QString)));
	connect(editorTwo, SIGNAL(textEdited(QString)), this, SLOT(TwoToNext(QString)));
	connect(editorThree, SIGNAL(textEdited(QString)), this, SLOT(ThreeToNext(QString)));
	connect(editorFour, SIGNAL(textChanged(QString)), this, SLOT(judgeInputFinished(QString)));
	connect(registerButton, SIGNAL(clicked()), this, SLOT(registerMe()));
	connect(nextButton, SIGNAL(clicked()), this, SLOT(hide()));
	connect(quitButton, SIGNAL(clicked()), this, SLOT(hide()));
	connect(nextButton, SIGNAL(clicked()), this, SIGNAL(nextButtonClicked()));
}
void RegisterWidget::OneToNext(QString text)
{
	if (text.length() >= 5)
		editorTwo->setFocus();
}
void RegisterWidget::TwoToNext(QString text)
{
	if (text.length() >= 5)
		editorThree->setFocus();
}
void RegisterWidget::ThreeToNext(QString text)
{
	if (text.length() >= 5)
		editorFour->setFocus();
}
void RegisterWidget::judgeInputFinished(QString text)
{
	if ((text.length() == 5) && (editorOne->text().length() == 5) && (editorTwo->text().length() == 5) && (editorThree->text().length() == 5))
		registerButton->setEnabled(true);
}
void RegisterWidget::registerMe() 
{
	std::array<int, 4> cpui;    //#include <array>
	// Calling __cpuid with 0x0 as the function_id argument  
	// gets the number of the highest valid function ID.  什么是function ID？
	__cpuid(cpui.data(), 0x0);    //cpui[0] = "funcition_id的最大值"
	int nIds_ = cpui[0];
	std::vector<std::array<int, 4>> data_;  //保存遍历到的所有cpui的值    
	for (int i = 0; i <= nIds_; ++i)
	{
		__cpuidex(cpui.data(), i, 0);
		data_.push_back(cpui);
	}
	//reinterpret_cast<int*>(vendor) //*reinterpret_cast<int*>(vendor)
	//索引0 0+4 0+8的值构成了CPU芯片的名称
	char vendor[0x20] = { 0 };
	*reinterpret_cast<int*>(vendor) = data_[0][1];
	*reinterpret_cast<int*>(vendor + 4) = data_[0][3];
	*reinterpret_cast<int*>(vendor + 8) = data_[0][2];  // vendor="GenuineIntel"    
	std::string vendor_ = vendor;
	bool isIntel_ = false;
	bool isAMD = false;
	if ("GenuineIntel" == vendor_)
	{
		isIntel_ = true;    //厂商为INTEL
	}
	else if ("AuthenticAMD" == vendor_)
	{
		isAMD = true;       //厂商为AMD
	}
	char vendor_serialnumber[0x14] = { 0 };
	sprintf_s(vendor_serialnumber, sizeof(vendor_serialnumber), "%08X%08X", data_[1][3], data_[1][0]);
	//用“wmic cpu get processorid”获取的结果进行比对，结果应该是一致的。    //"BFEBFBFF000906E9"	

	QByteArray mima = QCryptographicHash::hash(vendor_serialnumber, QCryptographicHash::Md5);
	QString hashCode = QString(mima.toHex()).toUpper();
	QString password = hashCode.left(20);

	QString yourRegCode = editorOne->text() + editorTwo->text() + 
		editorThree->text() + editorFour->text();
	if (yourRegCode == password)
	{
		regResult->setText(QString::fromLocal8Bit("注册成功！"));
		regResult->setStyleSheet("color:green;");
		nextButton->setEnabled(true);

		QFile file("./Licence/Licence.txt");
		QTextStream io(&file);
		file.open(QIODevice::Truncate);
		file.close();
		file.open(QIODevice::WriteOnly);
		io << password;
		file.setPermissions(QFileDevice::ReadOwner);
		file.close();
		//F50FD-C985E-07751-A1529	

	}
	else
	{
		regResult->setText(QString::fromLocal8Bit("注册码无效！"));
		regResult->setStyleSheet("color:red;");
		nextButton->setEnabled(false);
	}
}