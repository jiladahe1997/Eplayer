#include "NetEaseMusicClient.h"

#include <pthread.h>
#include <QDebug>
#include <iostream>
#include <string>
#include <random>

#include "nlohmann/json.hpp"

#include "Loading.h"
#include <thread>

string NetEaseMusicClient::cookie = string();

NetEaseMusicClient::NetEaseMusicClient(){

}

/**
 * c++中使用curl不能传对象成员函数，否则会报错
 */
//size_t NetEaseMusicClient::httpGetCB(void * buffer, size_t size, size_t nmemb, void * userp){
//    qInfo() << "触发http回调" << Qt::endl << "接收到内容:" << Qt::endl;
//    //qInfo() << (char*)buffer << Qt::endl;
//    return nmemb;
//}

static size_t httpGetCB2(void * buffer, size_t size, size_t nmemb, void * userp){
    qInfo() << "触发http回调" << Qt::endl << "接收到内容:" << Qt::endl;
    qInfo() << (char*)buffer << Qt::endl;
    struct HttpRes *httpRes = (struct HttpRes*)userp;
    httpRes->buffer = realloc(httpRes->buffer, httpRes->size+nmemb);
    memset((char*)httpRes->buffer+httpRes->size,0,nmemb);
    memcpy((char*)httpRes->buffer+httpRes->size,buffer,nmemb);
    httpRes->size+=nmemb;
    (void)size;
    return nmemb;
}

CURLcode NetEaseMusicClient::login(void) {
    using namespace std::placeholders;
    using namespace nlohmann;

    CURLcode ret = CURLE_OK;
    struct HttpRes httpRes = {
        .size = 0,
        .buffer = NULL
    };
    json j ;
    string str;

    //std::this_thread::sleep_for (std::chrono::seconds(2));    

    //loading->show();

    ret = this->httpGet("182.92.164.220:3000/login/cellphone?phone=13320949321&password=Freedom1997",
            //std::bind(&NetEaseMusicClient::httpGetCB,this,_1,_2,_3,_4),
            httpGetCB2,
            &httpRes);
    if(ret != CURLE_OK){
        qWarning("login https request fail, error code is %d",ret);
        goto error1;
    }
    /* 末尾加/0 */
    httpRes.buffer = std::realloc(httpRes.buffer ,httpRes.size+1);
    memset((char*)httpRes.buffer+httpRes.size,0,1);
    httpRes.size+=1;
    str=string((char*)(httpRes.buffer));
    //std::this_thread::sleep_for (std::chrono::seconds(2));    
    try
    {
        j = json::parse(str);
    }
    catch(const std::exception& e)
    {
        qWarning("json parse error, input is %s", (char*)httpRes.buffer);
        goto error1;
    }
    //std::cout << "登录成功,cookie:" << j["cookie"] << std::endl;
    //qInfo("%s", j["cookie"].get<std::string>());
    qInfo() << "登录成功,cookie:" << QString::fromStdString((j["cookie"].get<std::string>())) << Qt::endl;
    this->cookie = j["cookie"].get<std::string>();
    //loading->hide();
error1:
    free(httpRes.buffer);

    return ret;

}

CURLcode NetEaseMusicClient::httpGet(string url,
        //std::function<size_t(void *, size_t , size_t , void *)> cb,
        size_t (*cb)(void *,size_t,size_t,void*),
        void * userp){
    CURL * easyhandle = curl_easy_init();
    curl_easy_setopt(easyhandle, CURLOPT_URL, url.c_str());
    //curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, cb.target<size_t(*)(void *, size_t , size_t , void *)>());
    curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA,userp);
    curl_easy_setopt(easyhandle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(easyhandle, CURLOPT_COOKIE, this->cookie.c_str());

    return curl_easy_perform(easyhandle);
}

NetEaseMusicClient::SongInfo NetEaseMusicClient::getRandomMusicInfo(void) {
    /* 获取所有喜欢音乐 */
    struct HttpRes httpRes = {
        .size = 0,
        .buffer = NULL
    };
    CURLcode ret = this->httpGet("http://182.92.164.220:3000/likelist", httpGetCB2, &httpRes);
    if(ret != CURLE_OK){
        throw runtime_error("httpget error");
    }
    /* 末尾加/0 */
    httpRes.buffer = std::realloc(httpRes.buffer ,httpRes.size+1);
    memset((char*)httpRes.buffer+httpRes.size,0,1);
    httpRes.size+=1;
    long randomSongId=0;
    int songCount=0;
    nlohmann::json j1;
    try
    {
        j1 = nlohmann::json::parse((char*)(httpRes.buffer));
        songCount = j1["ids"].size();
    }
    catch(const std::exception& e)
    {
        qWarning("json parse error, input is %s", (char*)httpRes.buffer);
        throw &e;
    }

    /* 获取指定音乐的链接 */
    string url;
    while(url.size() == 0){
        random_device rd;
        std::mt19937 gen(rd());
        //default_random_engine e;
        uniform_int_distribution<int> u(0, songCount-1);
        int randomNum = u(gen);
        randomSongId = j1["ids"][randomNum].get<int64_t>();

        struct HttpRes httpRes2 = {
            .size = 0,
            .buffer = NULL
        };
        ret = this->httpGet("http://182.92.164.220:3000/song/url?id="+to_string(randomSongId), httpGetCB2, &httpRes2);
        if(ret != CURLE_OK){
            throw runtime_error("httpget error");
        }
        /* 末尾加/0 */
        httpRes2.buffer = std::realloc(httpRes2.buffer ,httpRes2.size+1);
        memset((char*)httpRes2.buffer+httpRes2.size,0,1);
        httpRes2.size+=1;
        try
        {
            nlohmann::json j;
            j = nlohmann::json::parse((char*)(httpRes2.buffer));
            if(j["data"][0]["url"].is_null())
                continue;
            url = j["data"][0]["url"].get<string>();
        }
        catch(const std::exception& e)
        {
            qWarning("json parse error, input is %s", (char*)httpRes2.buffer);
            throw &e;
        }    
    }

    string songName;
    string singer;
    string coverImgUrl;
    /* 获取歌曲基本信息 */
    {
        struct HttpRes httpRes = {
            .size = 0,
            .buffer = NULL
        };
        CURLcode ret = this->httpGet("http://182.92.164.220:3000/song/detail?ids="+to_string(randomSongId), httpGetCB2, &httpRes);
        if(ret != CURLE_OK){
            throw runtime_error("httpget error");
        }
        /* 末尾加/0 */
        httpRes.buffer = std::realloc(httpRes.buffer ,httpRes.size+1);
        memset((char*)httpRes.buffer+httpRes.size,0,1);
        httpRes.size+=1;
        nlohmann::json j;
        try
        {
            j = nlohmann::json::parse((char*)(httpRes.buffer));
            songName = j["songs"][0]["name"].get<string>();
            singer = j["songs"][0]["ar"][0]["name"].get<string>();
            coverImgUrl = j["songs"][0]["al"]["picUrl"].get<string>();
        }
        catch(const std::exception& e)
        {
            qWarning("json parse error, input is %s",(char*) httpRes.buffer);
            throw &e;
        }
    }
    return NetEaseMusicClient::SongInfo(
        songName,singer,coverImgUrl,url
    );
    // return "https://jiladahe1997-1256609098.cos.ap-chengdu.myqcloud.com/test/obj_w5zDlMODwrDDiGjCn8Ky_3035941127_7a87_6039_5d37_67c00da5ea62d90bbea8cd2aa8092aac.mp3";
}
