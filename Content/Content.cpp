#include "Content.h"

Content::Content(QWidget * parent){
    QLabel * coverImg = new QLabel(parent);
    coverImg->setPixmap(QPixmap(QString::fromUtf8(":/main/src=http _5b0988e595225.cdn.sohucs.com_q_70,c_zoom,w_640_images_20181017_32af56c42aea4c239f7867a95a3d5359.jpeg&refer=http _5b0988e595225.cdn.sohucs.com&app=2002&size=f9999,10000&q=a80&n=0&g=0n&fmt=jpeg.jpg")));
    coverImg->setGeometry(198,196,450,450);
    coverImg->setScaledContents(true);

    QLabel * songName = new QLabel(parent);
    songName->setText("有人");
    QFont font = songName->font();
    font.setPixelSize(58);
    songName->setFont(font);
    songName->setAlignment(Qt::AlignCenter);
    songName->setGeometry(863,198,180,60);
    //songName->setStyleSheet("QLabel { background-color : red; color : blue; }");

    QLabel * singer = new QLabel(parent);
    singer->setText("赵钶");
    font.setPixelSize(33);
    singer->setAlignment(Qt::AlignCenter);
    singer->setFont(font);
    singer->setGeometry(901,294,100,35);
    //singer->setStyleSheet("QLabel { background-color : red; color : blue; }");

    QLabel * controlPlay = new QLabel(parent);
    controlPlay->setPixmap(QPixmap(QString::fromUtf8(":/main/播放.png")));
    controlPlay->setGeometry(885,419,125,125);
    controlPlay->setScaledContents(true);

    QLabel * controlNext = new QLabel(parent);
    controlNext->setPixmap(QPixmap(QString::fromUtf8(":/main/下一首.png")));
    controlNext->setGeometry(1124,453,64,64);
    controlNext->setScaledContents(true);

    QLabel * controlPre = new QLabel(parent);
    controlPre->setPixmap(QPixmap(QString::fromUtf8(":/main/上一首.png")));
    controlPre->setGeometry(696,453,64,64);
    controlPre->setScaledContents(true);

    QLabel * process = new QLabel(parent);
    process->setPixmap(QPixmap(QString::fromUtf8(":/main/进度条.png")));
    process->setGeometry(814,618,282,14);
    process->setScaledContents(true);
}
