#include "mypainter.h"
#include <QFile>

MyPainter::MyPainter(QWidget *parent) : QWidget(parent){
    setMouseTracking(true);
    timer = new QTimer(parent);
    connect(timer, SIGNAL(timeout()), this, SLOT(createLine()));
}

void MyPainter::mousePressEvent(QMouseEvent *event){
    startpoint = event->pos();
    endpoint = startpoint;
    timer->start(200);
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

void MyPainter::writePoint(QTextStream &stream, QPoint *point, QPoint *firstpoint){
    stream << point->x()-firstpoint->x();
    stream << " ";
    stream << point->y()-firstpoint->y();
    stream << endl;
}
void MyPainter::save(QString filename){
    if(lines.size() < 1)
        return;
    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly) ){
        QPoint first = lines[0].p1();
        QTextStream stream( &file );
        for(int i=0 ; i<lines.size() ; i++){
            QPoint p = lines[i].p1();
            writePoint(stream, &p, &first);
        }
        QPoint p = lines[lines.size()-1].p2();
        writePoint(stream, &p, &first);
        file.close();
    }
    lines.clear();
    update();
}
