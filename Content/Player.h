#ifndef __PLAYER_H
#define __PLAYER_H

#include <QMediaPlayer>
#include <string>
#include <mutex>
#include <condition_variable>

using namespace std;

class Player : public QObject{
    Q_OBJECT
private:
    QMediaPlayer * qMediaPlayer;
    mutex mediaStatusMutex;
    condition_variable mediaStatusCV;
    QMediaPlayer::MediaStatus mediaStatus;
public:
    string nowPlayUrl;
    Player(void);
    bool playNewByUrl(string url);
    bool play(void);
    bool stop(void);
public slots: 
    void onMeidaChange(QMediaPlayer::MediaStatus status);
};

#endif   //__PLAYER_H