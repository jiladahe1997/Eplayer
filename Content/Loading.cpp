#include "Loading.h"
#include <QLabel>

Loading::Loading(int x, int y, string text) {
    this->label = new QLabel();
    this->label->setText(QString::fromStdString(text));
    this->label->setGeometry(x,y,DefaultWidth,DefaultHight);
    this->label->setAlignment(Qt::AlignCenter);
}

Loading::~Loading(){
    delete this->label;
}