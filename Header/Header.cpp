#include "Header.h"
#include <QFontDatabase>

Header::Header(QWidget * parent) : QWidget(parent)
{
    QWidget * container = new QWidget(parent);
    container->setGeometry(0,0,1280,72);
    container->setStyleSheet("QWidget {background-color: #eeeeee}");

    QFontDatabase database;
    const QStringList fontFamilies = database.families();
    qInfo("test");
    for(const auto & font :fontFamilies){
       qInfo("系统字体:%s",font.toStdString().c_str());
    }
    QLabel * username = new QLabel(parent);
    QFont font = username->font();
    font.setFamily("思源黑体 CN");
    font.setPixelSize(25);
    username->setFont(font);
    qInfo("当前username字体:%s", username->font().family().toStdString().c_str());
    username->setText("jiladahe1997");
    username->setAlignment(Qt::AlignTop);
    // username->setStyleSheet("QLabel { background-color : red; color : blue; }");
    username->setGeometry(20,20,150,40);

    QLabel * status = new QLabel(parent);
    status->setAlignment(Qt::AlignTop);
    status->setPixmap(QPixmap(QString::fromUtf8(":/main/online.png")));
    status->setGeometry(160,20,40,40);
    status->setScaledContents(true);
    // status->setStyleSheet("QLabel { background-color : red; color : blue; }");
    status->stackUnder(username);

    this->time = new QLabel(parent);
    this->time->setText("2021-01-13 20:10:00");
    this->time->setFont(font);
    this->time->setGeometry(700,20,250,40);
    // this->time->setStyleSheet("QLabel { background-color : red; color : blue; }");

    QLabel * battery = new QLabel(parent);
    battery->setAlignment(Qt::AlignTop);
    battery->setPixmap(QPixmap(QString::fromUtf8(":/main/full-battery.png")));
    battery->setGeometry(1050,20,66,40);
    battery->setScaledContents(true);
    // battery->setStyleSheet("QLabel { background-color : red; color : blue; }");

    QLabel * batteryText = new QLabel(parent);
    batteryText->setText("100%");
    batteryText->setFont(font);
    batteryText->setGeometry(1120,20,80,40);
    // batteryText->setStyleSheet("QLabel { background-color : red; color : blue; }");

    /* 启动时间自动更新 */
    HeaderTimeUpdaterThread * headerTimeUpdaterThread = new HeaderTimeUpdaterThread(this->time);
    headerTimeUpdaterThread->start();
}

HeaderTimeUpdaterThread::HeaderTimeUpdaterThread(QLabel * targetQLabel) {
    this->targetQLabel = targetQLabel;
}

void HeaderTimeUpdaterThread::run() {
    while(1){
        qInfo("更新时间\r\n");
        Time time;
        const std::string timeStr = time.getTimeString();
        this->targetQLabel->setText(QString::fromStdString(timeStr));
        QThread::msleep(100);
    }
}


