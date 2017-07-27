#ifndef SAVEPANEL
#define SAVEPANEL
#include <QWidget>
#include <QString>
#include <QLineEdit>

class SavePanel : public QWidget{
    Q_OBJECT
signals :
    void save(QString filename);
private :
    QLineEdit *editor;
public :
    SavePanel(QWidget *parent);
private slots :
    void pressed();
};

#endif // SAVEPANEL

