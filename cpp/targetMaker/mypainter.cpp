#include "mypainter.h"
#include <QFile>
#include <QTextStream>

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

void MyPainter::save(QString filename){
    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly) ){
        QTextStream stream( &file );
        for(QLine line : lines){
            stream << line.p1().x() <<" "<< line.p1().y() << endl;
        }
        QLine last = lines[lines.size()-1];
        stream << last.p2().x() <<" "<< last.p2().y() << endl;
        file.close();
    }
}
