#include "Player.h"

Player::Player(void) {
    this->nowPlayUrl = "";
    this->qMediaPlayer = new QMediaPlayer();
}

bool Player::play(string url) {
    /* check if url is same as nowUrl */
    if(url == this->nowPlayUrl) {

        return true;
    }
    this->nowPlayUrl = url;
    this->qMediaPlayer->setMedia(QUrl(QString::fromStdString(url)));
    this->qMediaPlayer->play();
    return true;
}

bool Player::stop(void) {
    this->qMediaPlayer->stop();
    return true;
}