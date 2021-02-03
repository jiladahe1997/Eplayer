#ifndef CONTENT_H
#define CONTENT_H

#include <QWidget>
#include <QLabel>

#include "NetEaseMusicClient.h"
#include "Player.h"

class Content : public QWidget {
    Q_OBJECT
private:
    NetEaseMusicClient * netEaseMusicClient;
    Player * player;
public:
    Content(QWidget * parent = nullptr);
public slots: 
    bool onPlayeClick(void);
};

#endif  // CONTENT_H

