#include <QApplication>
#include <QLabel>
#include "mypainter.h"

int main(int argc, char *argv[])
{
    const int windowHeight = 400;
    const int windowWidth  = 300;

    QApplication app(argc, argv);
    // create window
    QWidget fenetre;
    fenetre.setFixedSize(windowHeight,windowWidth);
    fenetre.move(100,100);

    MyPainter myp(&fenetre);
    myp.setFixedSize(windowHeight-5,windowWidth-5);
    fenetre.show();

    return app.exec();
}
