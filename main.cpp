#include "mainwindow.h"
#include "Header/Header.h"
#include "Content/Content.h"

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

#include "curl/curl.h"
QMainWindow *mainWindow;
int main(int argc, char *argv[])
{
    curl_global_init(CURL_GLOBAL_ALL);

    QApplication a(argc, argv);
    mainWindow=new QMainWindow();

    /* 渲染其他窗口 */
    Header header(mainWindow);
    Content content(mainWindow);

    mainWindow->resize(1280,800);
    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsProxyWidget *w1 = scene->addWidget(mainWindow);
    //w1->setRotation(90);
    QGraphicsView *view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->resize(800, 1280);
    view->setAutoFillBackground(true);
    QPalette palette(mainWindow->palette());
    palette.setColor(QPalette::Window, Qt::white);
    mainWindow->setPalette(palette);

    // view->show();
    view->show();

    return a.exec();
}
