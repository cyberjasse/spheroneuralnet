#ifndef MYPAINTER
#define MYPAINTER
#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QVector>
#include <QLine>
#include <QPen>
#include <QMouseEvent>
#include <QPaintEvent>

// code from https://stackoverflow.com/questions/11683976/qt-4-7-drawing-a-2-point-line-with-dynamic-feedback-using-paintevent
class MyPainter : public QWidget{
private :
    QPoint startpoint;
    QVector<QLine> lines;
public :
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    MyPainter(QWidget *parent);
};

#endif // MYPAINTER

