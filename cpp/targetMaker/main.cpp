#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>
#include "mypainter.h"
#include "savepanel.h"

int main(int argc, char *argv[])
{
    const int windowHeight = 400;
    const int windowWidth  = 300;
    const int buttonsHeight = 20;

    // the window
    QApplication app(argc, argv);
    QWidget window;
    window.setFixedSize(windowHeight,windowWidth);
    window.move(100,100);
    QVBoxLayout *layout = new QVBoxLayout;

    // the paint panel
    MyPainter myp(&window);
    myp.setFixedSize(windowHeight-buttonsHeight,windowWidth);
    layout->addWidget(&myp);

    // the filename entry panel
    SavePanel savepanel(&window);
    savepanel.setFixedWidth(windowWidth);
    QObject::connect(&savepanel, SIGNAL(save(QString)), &myp, SLOT(save(QString)));
    layout->addWidget(&savepanel);

    window.setLayout(layout);
    window.show();
    return app.exec();
}
