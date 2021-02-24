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
    vector<NetEaseMusicClient::SongInfo> playHistory;
    void playPreSong(void);
    void playNextSong(void);
    void playSongByUrl(string url);
    void playStart(void);
    void changeUiBySongInfo(NetEaseMusicClient::SongInfo songInfo);
    void updateProcessText();  //线程函数，每秒更新进度条文字
public:
    QPushButton *controlPlay;
    QLabel *coverImg;
    QLabel *songName;
    QLabel *singer;
    QLabel *processText;       //进度条文字
    NetEaseMusicClient * netEaseMusicClient;
    Player * player;
    Content(QWidget * parent = nullptr);
public slots: 
    bool onPlayeClick(void);
    void onNextSongClick(void);
    void onPreSongClick(void);
    void onPlayerStatusChange(QMediaPlayer::MediaStatus status);
};

#endif  // CONTENT_H

