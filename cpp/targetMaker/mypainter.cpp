#include "mypainter.h"

MyPainter::MyPainter(QWidget *parent) : QWidget(parent){
    setMouseTracking(true);
    timer = new QTimer(parent);
    connect(timer, SIGNAL(timeout()), this, SLOT(createLine()));
}

void MyPainter::mousePressEvent(QMouseEvent *event){
    startpoint = event->pos();
    endpoint = startpoint;
    timer->start(1000);
}

void MyPainter::mouseReleaseEvent(QMouseEvent *event){
    endpoint = event->pos();
    timer->stop();
}

void MyPainter::paintEvent(QPaintEvent *event){
    //prepare the painter
    QPainter p(this);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    p.setPen(pen);

    //paint lines
    p.drawLines(lines);
}

void MyPainter::mouseMoveEvent(QMouseEvent *event){
    endpoint = event->pos();
}

void MyPainter::createLine(){
    QLine line = QLine(startpoint, endpoint);
    lines.append(line);
    startpoint = endpoint;
    update();
}
