#-------------------------------------------------
#
# Project created by QtCreator 2017-06-29T09:39:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FilSysGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fileSystem.cpp \
    fileslist.cpp \
    mainFunctions.cpp \
    inorderlist.cpp \
    pie.cpp \
    dialog.cpp \
    dialog2.cpp \
    list_files.cpp

HEADERS  += mainwindow.h \
    fileSystem.h \
    fileslist.h \
    mainFunctions.h \
    inorderlist.h \
    pie.h \
    dialog.h \
    dialog2.h \
    list_files.h \
    dirent_W.h

FORMS    += mainwindow.ui \
    dialog.ui \
    dialog2.ui \
    list_files.ui

DISTFILES += \
    ../Desktop/pic.jpg \
    file.png \
    Folder.png
