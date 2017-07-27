#include <QApplication>
#include <QLabel>
#include "mypainter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // create window
    QWidget fenetre;
    fenetre.setFixedSize(300,150);
    fenetre.move(100,100);
    // create message displayer
    MyPainter myp(&fenetre);
    myp.setFixedSize(280,130);
    fenetre.show();

    return app.exec();
}
