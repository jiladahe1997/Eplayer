#include "Loading.h"
#include <QLabel>
#include <QMainWindow>
#include <QGraphicsView>

QLabel * Loading::labelImg = nullptr;
QLabel * Loading::labelText = nullptr;
QWidget * Loading::maskWidget = nullptr;
vector<Loading::LoadingCallRecord*> *Loading::callRecord = new vector<Loading::LoadingCallRecord*>;

Loading::Loading(string text) {
    if(this->labelImg == nullptr || this->labelText == nullptr){
        throw runtime_error("必须先在Contnet中初始化label");
    }
    this->labelText->setText(QString::fromStdString(text));
    maskWidget->show();
    LoadingCallRecord *loadingCallRecord = new LoadingCallRecord(this, text);
    this->callRecord->push_back(loadingCallRecord);
}

Loading::~Loading(){    
    if(this->labelImg == nullptr || this->labelText == nullptr){
        throw runtime_error("必须先在Contnet中初始化label");
    }
    for(vector<LoadingCallRecord*>::iterator iter=this->callRecord->begin();iter!=this->callRecord->end();){
        if((*iter)->_loading == this){
            delete(*iter);
            iter = this->callRecord->erase(iter);
        }else{
            ++iter;
        }
    }
    size_t leftSize = this->callRecord->size();
    if(leftSize != 0){
        this->labelText->setText(QString::fromStdString(this->callRecord->back()->_text));
    }
    else{
        maskWidget->hide();
    }
}

void Loading::init(QWidget *parent){
    Loading::maskWidget = new QWidget(parent);
    Loading::maskWidget->setStyleSheet("background-color: rgba(255, 255, 255, 180);");//设置颜色及不透明度
    Loading::maskWidget->setFixedSize(1280,800);//设置窗口大小
    // Loading::maskWidget ->setVisible(false);//初始状态下隐藏，待需要显示时使用m_pMaskLayer->setVisible(true);
    Loading::labelImg = new QLabel(maskWidget);
    QPalette pal = labelImg->palette();
    pal.setColor(QPalette::Window, QColor(QRgba64::fromRgba(0,0,0,180)));
    labelImg->setPalette(pal);
    Loading::labelImg->setPixmap(QPixmap(QString::fromUtf8(":/main/loading.png")));
    Loading::labelImg->setGeometry(640,200,200,200);
    Loading::labelImg->setStyleSheet("background-color: rgba(255, 255, 255, 180);");
    Loading::labelImg->setScaledContents(true);
    Loading::labelText = new QLabel(maskWidget);
    Loading::labelText->setText("加载中");
    Loading::labelText->setGeometry(640,400,200,40);
    labelText->setAutoFillBackground(true);
    Loading::labelText->setStyleSheet("background-color: rgba(255, 255, 255, 180);");
    Loading::labelText->setAlignment(Qt::AlignCenter);
    Loading::labelText->raise();
    maskWidget->hide();
}
