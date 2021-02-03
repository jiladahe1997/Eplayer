#include "NetEaseMusicClient.h"
#include <pthread.h>
#include <QDebug>
#include "nlohmann/json.hpp"
#include <iostream>
#include <string>

NetEaseMusicClient::NetEaseMusicClient(){
    CURLcode ret = this->login();
    if(ret != CURLE_OK) {
        qWarning("登录失败!");
    }
}

size_t NetEaseMusicClient::httpGetCB(void * buffer, size_t size, size_t nmemb, void * userp){
    qInfo() << "触发http回调" << Qt::endl << "接收到内容:" << Qt::endl;
    //qInfo() << (char*)buffer << Qt::endl;
    return nmemb;
}

static size_t httpGetCB2(void * buffer, size_t size, size_t nmemb, void * userp){
    qInfo() << "触发http回调" << Qt::endl << "接收到内容:" << Qt::endl;
    qInfo() << (char*)buffer << Qt::endl;
    struct HttpRes *httpRes = (struct HttpRes*)userp;
    httpRes->buffer = realloc(httpRes->buffer, httpRes->size+nmemb);
    memset((char*)httpRes->buffer+httpRes->size,0,nmemb);
    memcpy((char*)httpRes->buffer+httpRes->size,buffer,nmemb);
    httpRes->size+=nmemb;
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

    ret = this->httpGet("182.92.164.220:3000/login/cellphone?phone=13320949321&password=Freedom1997",
            //std::bind(&NetEaseMusicClient::httpGetCB,this,_1,_2,_3,_4),
            httpGetCB2,
            &httpRes);
    if(ret != CURLE_OK){
        qWarning("login https request fail, error code is %d",ret);
        goto error1;
    }
    str=string((char*)(httpRes.buffer));
    try
    {
        j = json::parse(str);
    }
    catch(const std::exception& e)
    {
        qWarning("json parse error, input is %s",httpRes.buffer);
        goto error1;
    }
    //std::cout << "登录成功,cookie:" << j["cookie"] << std::endl;
    //qInfo("%s", j["cookie"].get<std::string>());
    qInfo() << "登录成功,cookie:" << QString::fromStdString((j["cookie"].get<std::string>())) << Qt::endl;

error1:
    free(httpRes.buffer);

error:
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
    return curl_easy_perform(easyhandle);
}

string NetEaseMusicClient::getRandomMusicUrl(void) {
    /* 获取所有喜欢音乐 */
    //this->httpGet("http://182.92.164.220:3000/likelist", this->httpGetCB);
    // return ;
    return "https://jiladahe1997-1256609098.cos.ap-chengdu.myqcloud.com/test/obj_w5zDlMODwrDDiGjCn8Ky_3035941127_7a87_6039_5d37_67c00da5ea62d90bbea8cd2aa8092aac.mp3";
}

// NetEaseMusicClient::HttpRes::HttpRes(void){
//     this->count=0;
//     this->buffer=malloc(CURL_MAX_WRITE_SIZE);
// }
