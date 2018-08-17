#ifndef RIGISTERWIDGET_H
#define RIGISTERWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <array>
#include <intrin.h>
#include <vector>
#include <cstring>

class RegisterWidget : public QWidget
{
	Q_OBJECT

public:
	RegisterWidget(QWidget *parent = 0);
	~RegisterWidget();
protected:
	void paintEvent(QPaintEvent *ev);
private slots:
	void OneToNext(QString text);
	void TwoToNext(QString text);
	void ThreeToNext(QString text);
	void judgeInputFinished(QString text);
	void registerMe();
signals:
	void nextButtonClicked();
private:
	void initUi();
	void initConect();

	QLineEdit *editorOne;
	QLineEdit *editorTwo;
	QLineEdit *editorThree;
	QLineEdit *editorFour;
	QLabel *regCodeLabel;
	QLabel *line1;
	QLabel *line2;
	QLabel *line3;
	QLabel *regResult;
	QAbstractButton *registerButton;
	QAbstractButton *quitButton;
	QAbstractButton *nextButton;
};

#endif // RIGISTERWIDGET_H
