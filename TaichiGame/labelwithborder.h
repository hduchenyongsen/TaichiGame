#ifndef LABELWITHBORDER_H
#define LABELWITHBORDER_H

#include <QWidget>

class LabelWithBorder : public QWidget
{
	Q_OBJECT

public:
	LabelWithBorder(QWidget *parent);
	LabelWithBorder(QWidget *parent,int borderNo);
	~LabelWithBorder();
protected:
	void paintEvent(QPaintEvent *ev);

private:
	QImage borderImg;
};

#endif // LABELWITHBORDER_H
