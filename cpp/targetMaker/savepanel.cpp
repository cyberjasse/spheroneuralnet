#include "savepanel.h"
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>

SavePanel::SavePanel(QWidget *parent) : QWidget(parent){
    QPushButton *button = new QPushButton("Save");
    editor = new QLineEdit(parent);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(button);
    layout->addWidget(editor);
    setLayout(layout);
    QObject::connect(button, SIGNAL(pressed()), this, SLOT(pressed()));
}

void SavePanel::pressed(){
    emit save(editor->text());
}
