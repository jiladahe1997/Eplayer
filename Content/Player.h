#ifndef __PLAYER_H
#define __PLAYER_H

#include <QMediaPlayer>
#include <string>

using namespace std;

class Player {
private:
    QMediaPlayer * qMediaPlayer;
public:
    string nowPlayUrl;
    Player(void);
    bool play(string url);
    bool stop(void);
};

#endif   //__PLAYER_H