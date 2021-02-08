#ifndef CONTENT_H
#define CONTENT_H

#include <QWidget>
#include <QLabel>

#include "NetEaseMusicClient.h"
#include "Player.h"
#include <QPushButton>

class Content : public QWidget {
    Q_OBJECT
private:
    enum PlayStatus {
        INIT,
        STOP,
        PLAYTING
    };
    PlayStatus playStatus;
public:
    QPushButton *controlPlay;
    QLabel *coverImg;
    QLabel *songName;
    QLabel *singer;
    NetEaseMusicClient * netEaseMusicClient;
    Player * player;
public:
    Content(QWidget * parent = nullptr);
public slots: 
    bool onPlayeClick(void);
};

#endif  // CONTENT_H

