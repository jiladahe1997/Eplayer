/**
 * Loading 框设计思路
 * 创造：
 *      1.任何人都可以创造Loading框，并不是单例的
 *      2.创造的时候需要指定的参数
 *          a. Loading框的位置
 *          b. Loading框的附带文字
 *      3.全局只存在一个QLabel实例
 * 
 * 结构：
 *      1.分为两部分
 *          a.对外提供部分，主要用于显示和隐藏loading框
 *          b.对友元函数使用，用于初始化QLabel实例
 * 
 * 行为：
 *      1.Loading框的显示：新建对象时就会显示
 *      2.Loading框的消失：删除对象时就会消失
 *      3.暂不考虑 Loading框的层级关系，多个Loading框覆盖？
 *      4.内存资源管理
 *      5.线程安全性？多线程创建loading框怎么处理
 */
#include <string>
#include <QLabel>
#include "Content.h"

using namespace std;


class Loading
{
public:
    class LoadingCallRecord;
private:
    static const int DefaultWidth = 1080;
    static const int DefaultHight = 800;
    friend class Content;
    static QLabel *labelImg;
    static QLabel *labelText;
    static QWidget *maskWidget;
    static void init(QWidget *parent);
public:
    class LoadingCallRecord{
    public:
        Loading * _loading;
        string _text;
        LoadingCallRecord(
            Loading * loading, 
            string text):
            _loading(loading),
            _text(text){};
    };
    static vector<Loading::LoadingCallRecord*> *callRecord;
public:
    explicit Loading(string text);;
    ~Loading();
};
