QT       += core gui sql uitools
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    databasemanager.cpp \
    graph.cpp \
    main.cpp \
    mainwindow.cpp \
    userpanel.cpp

HEADERS += \
    databasemanager.h \
    graph.h \
    mainwindow.h \
    userpanel.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
