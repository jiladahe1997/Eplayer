#include "Content.h"
#include <QPushButton>
#include <string>
#include <thread>
#include "Loading.h"

using namespace std;
Content::Content(QWidget * parent){
    QLabel * coverImg = new QLabel(parent);
    coverImg->setPixmap(QPixmap(QString::fromUtf8(":/main/src=http _5b0988e595225.cdn.sohucs.com_q_70,c_zoom,w_640_images_20181017_32af56c42aea4c239f7867a95a3d5359.jpeg&refer=http _5b0988e595225.cdn.sohucs.com&app=2002&size=f9999,10000&q=a80&n=0&g=0n&fmt=jpeg.jpg")));
    coverImg->setGeometry(198,196,450,450);
    coverImg->setScaledContents(true);

    QLabel * songName = new QLabel(parent);
    songName->setText("Eplayer MP3播放器");
    QFont font = songName->font();
    font.setPixelSize(58);
    songName->setFont(font);
    songName->setAlignment(Qt::AlignCenter);
    songName->setGeometry(863,198,180,60);
    //songName->setStyleSheet("QLabel { background-color : red; color : blue; }");

    QLabel * singer = new QLabel(parent);
    singer->setText("author: renmingrui");
    font.setPixelSize(33);
    singer->setAlignment(Qt::AlignCenter);
    singer->setFont(font);
    singer->setGeometry(901,294,100,35);
    //singer->setStyleSheet("QLabel { background-color : red; color : blue; }");

    QPushButton * controlPlay = new QPushButton(parent);
    QPixmap pixmap = QPixmap(QString::fromUtf8(":/main/播放.png"));
    controlPlay->setIcon(pixmap);
    controlPlay->setGeometry(885,419,125,125);
    controlPlay->setIconSize(QSize(125,125));
    controlPlay->setFlat(true);
    connect(controlPlay, &QPushButton::clicked, this, &Content::onPlayeClick);

    QPushButton * controlNext = new QPushButton(parent);
    pixmap = QPixmap(QString::fromUtf8(":/main/下一首.png"));
    controlNext->setIcon(pixmap);
    controlNext->setGeometry(1124,453,64,64);
    controlNext->setIconSize(QSize(64,64));
    controlNext->setFlat(true);

    QPushButton * controlPre = new QPushButton(parent);
    pixmap = QPixmap(QString::fromUtf8(":/main/上一首.png"));
    controlPre->setIcon(pixmap);
    controlPre->setGeometry(696,453,64,64);
    controlPre->setIconSize(QSize(64,64));
    controlPre->setFlat(true);

    QLabel * process = new QLabel(parent);
    process->setPixmap(QPixmap(QString::fromUtf8(":/main/进度条.png")));
    process->setGeometry(814,618,282,14);
    process->setScaledContents(true);

    Loading::init(parent);


    this->netEaseMusicClient = new NetEaseMusicClient();
    this->player = new Player();
}

void playClick(Content *content) {
    qInfo() << "点击事件触发" << Qt::endl;
    CURLcode ret = content->netEaseMusicClient->login();
    if(ret != CURLE_OK) {
        qWarning("登录失败!");
    }
    string url = content->netEaseMusicClient->getRandomMusicUrl();
    content->player->play(url);
}

bool Content::onPlayeClick(void){
    std::thread t1(playClick,this);
    t1.detach();
    return true;
}
