QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += conan_basic_setup
include(./build/conanbuildinfo.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Content/Content.cpp \
    Content/NetEaseMusicClient.cpp \
    Content/Player.cpp \
    Header/DateTime.cpp \
    Header/Header.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Content/Content.h \
    Content/NetEaseMusicClient.h \
    Content/Player.h \
    Header/DateTime.h \
    Header/Header.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

DISTFILES += \
    ../../../Desktop/IMG/loading.png
