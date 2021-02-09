#include "DateTime.h"
#include <QProcess>

Time::Time(void){

}

std::string Time::getTimeString(){
    QProcess * process = new QProcess();
    process->start("date",QStringList()<<"+\"%Y-%m-%d %H:%M:%S\"");
    process->waitForFinished(-1);
    QByteArray res = process->readAll();
    delete(process);
    qInfo("接收到命令行返回 %s \r\n",res.toStdString().c_str());
    return res.toStdString().substr(1, 19);
}
