TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    server.cpp \
    userlist.cpp \
    user.cpp \
    dbmysql.cpp \
    thread.cpp
LIBS += -lmysqlclient \
    -lpthread

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    server.h \
    userlist.h \
    user.h \
    dbmysql.h \
    thread.h \
    msg.h

