#include "labelwithborder.h"
#include <QPainter>

LabelWithBorder::LabelWithBorder(QWidget *parent)
	: QWidget(parent)
{
}
LabelWithBorder::LabelWithBorder(QWidget *parent, int borderNo)
	: QWidget(parent)
{
	switch (borderNo)
	{
	case 0:
		borderImg.load(":/TaichiGame/Resources/xiakuang.png"); 
		break;
	case 1:
		borderImg.load(":/TaichiGame/Resources/shangkuang.png");
		break;
	default:
		break;
	}
}

LabelWithBorder::~LabelWithBorder()
{

}

void LabelWithBorder::paintEvent(QPaintEvent *ev)
{
	QPainter pp(this);
	QRect Temp(0, 0, this->width(), this->height());
	pp.drawImage(Temp, borderImg);
}