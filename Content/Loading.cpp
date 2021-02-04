#include "Loading.h"
#include <QLabel>
#include <QMainWindow>
#include <QGraphicsView>
extern QMainWindow *mainWindow;

Loading::Loading(int x, int y, string text) {
    this->label = new QLabel(mainWindow);
    QFont font = this->label->font();
    font.setFamily("思源黑体 CN");
    font.setPixelSize(25);
    this->label->setFont(font);
    this->label->setText("jiladahe1997");
    this->label->setGeometry(20,60,150,40);
    this->label->setAlignment(Qt::AlignCenter);
    this->label->raise();
}

Loading::~Loading(){
    delete this->label;
}

void Loading::show(void) {
    this->label->show();
}

void Loading::hide(void) {
    this->label->hide();
}
