QT       += core gui sql uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    databasemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    userpanel.cpp

HEADERS += \
    databasemanager.h \
    mainwindow.h \
    userpanel.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
