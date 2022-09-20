QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCPServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sifrecoz.cpp \
    tcpsever.cpp

HEADERS  += mainwindow.h \
    sifrecoz.h \
    tcpsever.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../cryptopp870/release/ -lcryptopp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../cryptopp870/debug/ -lcryptopp
else:unix: LIBS += -L$$PWD/../../cryptopp870/ -lcryptopp

INCLUDEPATH += $$PWD/../../cryptopp870
DEPENDPATH += $$PWD/../../cryptopp870
