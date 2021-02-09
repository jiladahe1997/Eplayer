#include "Player.h"
#include "Loading.h"
#include <QEventLoop>

Player::Player(void):
    qMediaPlayer(new QMediaPlayer),
    mediaStatusMutex(),
    mediaStatusCV(),
    mediaStatus(QMediaPlayer::MediaStatus::UnknownMediaStatus),
    nowPlayUrl()
{
    QObject::connect(this->qMediaPlayer, &QMediaPlayer::mediaStatusChanged,
                    this, &Player::onMeidaChange);
}

bool Player::playNewByUrl(string url) {
    /* check if url is same as nowUrl */
    if(url == this->nowPlayUrl) {
        return true;
    }
    this->nowPlayUrl = url;
    Loading *loading = new Loading("加载音频中...");
    this->qMediaPlayer->setMedia(QUrl(QString::fromStdString(url)));
    /* 等待音频加载完毕 */
    std::unique_lock<std::mutex> lk(this->mediaStatusMutex);
    this->mediaStatusCV.wait(lk, 
        [this]{return this->mediaStatus == QMediaPlayer::MediaStatus::LoadedMedia;});

    qInfo("音频加载完毕，总时长:%d", (int)this->qMediaPlayer->duration());
    delete(loading);
    this->qMediaPlayer->play();

    return true;
}

bool Player::play(void) {
    this->qMediaPlayer->play();
    return true;
}

bool Player::stop(void) {
    this->qMediaPlayer->pause();
    return true;
}

void Player::onMeidaChange(QMediaPlayer::MediaStatus status) {
    std::lock_guard<std::mutex> lk(this->mediaStatusMutex);
    qInfo("media status变化回调,变化前:%d,变化后:%d",this->mediaStatus,status);
    this->mediaStatus = status;
    this->mediaStatusCV.notify_all();

    /* 当前这首播放完毕，自动播放下一首 */
    if(status == QMediaPlayer::MediaStatus::EndOfMedia) {
        emit PlayerStatusChange(status);
    }
}
