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
#include <QTimer>
#include <QString>

// code from https://stackoverflow.com/questions/11683976/qt-4-7-drawing-a-2-point-line-with-dynamic-feedback-using-paintevent
class MyPainter : public QWidget{
    Q_OBJECT
private :
    QPoint startpoint;
    QPoint endpoint;
    QVector<QLine> lines;
    QTimer *timer;
public :
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    MyPainter(QWidget *parent);
public slots :
    void createLine();
    void save(QString filename);
};

#endif // MYPAINTER

