/**
 * Loading 框设计思路
 * 创造：
 *      1.任何人都可以创造Loading框，并不是单例的
 *      2.创造的时候需要指定的参数
 *          a. Loading框的位置
 *          b. Loading框的附带文字
 * 
 * 结构：
 *      1.继承QLabel类
 *      2.采用特定的一张静态图片（暂时，后续考虑换成gif)
 * 
 * 行为：
 *      1.Loading框的显示：新建对象时就会显示
 *      2.Loading框的消失：删除对象时就会消失
 *      3.暂不考虑 Loading框的层级关系，多个Loading框覆盖？
 *      4.内存资源管理
 */
#include <string>

using namespace std;

const int DefaultWidth = 1080;
const int DefaultHight = 800;

class Loading
{
private:
    QLabel *label;
public:
    Loading(int x, int y, string text);
    ~Loading();
};
