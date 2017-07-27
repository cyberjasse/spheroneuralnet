#include "mypainter.h"

MyPainter::MyPainter(QWidget *parent) : QWidget(parent){
    setMouseTracking(true);
}

void MyPainter::mousePressEvent(QMouseEvent *event){
    startpoint = event->pos();
}

void MyPainter::mouseReleaseEvent(QMouseEvent *event){
    QLine line = QLine(startpoint, event->pos());
    lines.append(line);
    update();
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
