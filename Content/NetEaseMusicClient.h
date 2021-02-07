#ifndef __NetEaseMusicClient_H
#define __NetEaseMusicClient_H

#include <string>
#include <QObject>

#include "curl/curl.h"
#include <functional>

using namespace std;

struct HttpRes{
    size_t size;
    void * buffer;
    //HttpRes(void);
};
class NetEaseMusicClient {
private:
    /**
     * @brief 此函数执行一次http请求
     * @note  此函数是同步函数，会阻塞直到http请求完成
     */
    CURLcode httpGet(string url,
        //std::function<size_t(void *, size_t , size_t , void *)> cb,
        size_t (*cb)(void *,size_t,size_t,void*),
        void * data_buffer);
    size_t httpGetCB(void * buffer, size_t szie, size_t nmemb, void * userp);
private:
    static string cookie;
public:
    CURLcode login(void);
    NetEaseMusicClient(void);
    string getRandomMusicUrl(void);
};

#endif // NetEaseMusicClient
