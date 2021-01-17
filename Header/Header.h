#ifndef HEADER_H
#define HEADER_H

#include <QWidget>
#include <QLabel>

class Header : public QWidget
{
    Q_OBJECT
public:
    QLabel * time;
    explicit Header(QWidget * parent = nullptr);
};

#include <QThread>
#include "DateTime.h"
class HeaderTimeUpdaterThread : public QThread {
    Q_OBJECT
    void run() override;
public:
    HeaderTimeUpdaterThread(QLabel * targetQLabel);
private:
    QLabel * targetQLabel;
};





#endif // HEADER_H
