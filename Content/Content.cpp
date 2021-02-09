#include "Content.h"
#include <string>
#include <thread>
#include "Loading.h"
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>

using namespace std;
Content::Content(QWidget * parent):
    playStatus(INIT)
    {
    coverImg = new QLabel(parent);
    coverImg->setPixmap(QPixmap(QString::fromUtf8(":/main/src=http _5b0988e595225.cdn.sohucs.com_q_70,c_zoom,w_640_images_20181017_32af56c42aea4c239f7867a95a3d5359.jpeg&refer=http _5b0988e595225.cdn.sohucs.com&app=2002&size=f9999,10000&q=a80&n=0&g=0n&fmt=jpeg.jpg")));
    coverImg->setGeometry(150,196,450,450);
    coverImg->setScaledContents(true);

    songName = new QLabel(parent);
    string str="Eplayer MP3播放器";
    songName->setText(QString::fromStdString(str));
    QFont font = songName->font();
    font.setPixelSize(58);
    songName->setFont(font);
    songName->setAlignment(Qt::AlignCenter);
    songName->setGeometry(672,150,550,60);
    QFontMetrics metrics(songName->font());
    QString elidedText = metrics.elidedText(QString::fromStdString(str), Qt::ElideRight, songName->width());
    songName->setText(elidedText);
    // songName->setStyleSheet("QLabel { background-color : red; color : blue; }");

    singer = new QLabel(parent);
    singer->setText("author: renmingrui");
    font.setPixelSize(33);
    singer->setAlignment(Qt::AlignCenter);
    singer->setFont(font);
    singer->setGeometry(700,294,500,35);
    metrics = QFontMetrics(songName->font());
    elidedText = metrics.elidedText("author: renmingrui", Qt::ElideRight, singer->width());
    singer->setText(elidedText);

    // singer->setStyleSheet("QLabel { background-color : red; color : blue; }");

    controlPlay = new QPushButton(parent);
    QPixmap pixmap = QPixmap(QString::fromUtf8(":/main/播放.png"));
    controlPlay->setIcon(pixmap);
    controlPlay->setGeometry(885,419,125,125);
    controlPlay->setIconSize(QSize(125,125));
    controlPlay->setFlat(true);

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

    connect(controlPlay, &QPushButton::clicked, this, &Content::onPlayeClick);
    connect(controlNext, &QPushButton::clicked, this, &Content::onNextSongClick);
    connect(controlPre, &QPushButton::clicked, this, &Content::onPreSongClick);
    connect(player, &Player::PlayerStatusChange, this, &Content::onPlayerStatusChange);
}

void Content::playStart(void) {
    try{
        Loading * loading = new Loading("登录中");
        CURLcode ret = this->netEaseMusicClient->login();
        if(ret != CURLE_OK) {
            qWarning("登录失败!");
        }
        delete(loading);
        this->playNextSong();
    } catch(std::exception &e){
        qFatal("播放错误");
    }
}

bool Content::onPlayeClick(void){
    QPixmap pixmap1 = QPixmap(QString::fromUtf8(":/main/pause.png"));
    QPixmap pixmap2 = QPixmap(QString::fromUtf8(":/main/播放.png"));

    switch (this->playStatus)
    {
    case Content::PlayStatus::INIT:
        {
        std::thread t1(&Content::playStart,this);
        t1.detach();
        this->playStatus = PLAYTING;
        }
        break;
    case Content::PlayStatus::PLAYTING:
        this->player->stop();
        this->playStatus=STOP;
        this->controlPlay->setIcon(pixmap2);
        break;
    case Content::PlayStatus::STOP:
        this->player->play();
        this->playStatus=PLAYTING;
        this->controlPlay->setIcon(pixmap1);
        break;    
    default:
        break;
    }

    return true;
}

void Content::onPlayerStatusChange(QMediaPlayer::MediaStatus status){
    if(status == QMediaPlayer::MediaStatus::EndOfMedia){
        std::thread t(&Content::playNextSong, this);
        t.detach();
    }
    return;
}

void Content::onNextSongClick(void) {
    std::thread t(&Content::playNextSong, this);
    t.detach();
    return;
}

void Content::onPreSongClick(void) {

    std::thread t(&Content::playPreSong, this);
    t.detach();
    return;
}

void Content::playSongByUrl(string url){
    this->player->playNewByUrl(url);
}

void Content::playPreSong(void) {
    /* 从history中取上一首 */
    if(!(this->playHistory.size() > 1)) {
        Loading loading("无上一首歌曲信息");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return;
    }
    NetEaseMusicClient::SongInfo songInfo = this->playHistory[this->playHistory.size()-2];
    this->changeUiBySongInfo(songInfo);
    this->playSongByUrl(songInfo._url);
}

void Content::changeUiBySongInfo(NetEaseMusicClient::SongInfo songInfo){
    QFontMetrics metrics(this->songName->font());
    QString elidedText = metrics.elidedText(
        QString::fromStdString(songInfo._songName),
        Qt::ElideRight,
        this->songName->width()
    );
    this->songName->setText(elidedText);
    metrics = QFontMetrics(this->singer->font());
    elidedText = metrics.elidedText(
        QString::fromStdString(songInfo._singer),
        Qt::ElideRight,
        this->singer->width()
    );
    this->singer->setText(elidedText);

    QNetworkAccessManager manager;
    QEventLoop loop; 
    QNetworkReply *reply = manager.get(QNetworkRequest(QString::fromStdString(songInfo._coverImgUrl)));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray jpegData = reply->readAll();
    QPixmap pixmap;
    pixmap.loadFromData(jpegData);
    this->coverImg->setPixmap(pixmap);
    /* 替换播放按钮图片 */
    pixmap = QPixmap(QString::fromUtf8(":/main/pause.png"));
    this->controlPlay->setIcon(pixmap);
}


void Content::playNextSong(void) {
    Loading *loading = new Loading("获取歌曲信息中");
    NetEaseMusicClient::SongInfo songInfo = this->netEaseMusicClient->getRandomMusicInfo();

    /* 替换歌曲名、封面图 */
    this->changeUiBySongInfo(songInfo);


    this->playHistory.push_back(songInfo);
    this->playSongByUrl(songInfo._url);
    delete(loading);
}
