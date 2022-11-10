QT       += core gui
QT       += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 0.0.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =
CONFIG(debug,debug|release) {
    DESTDIR = $${_PRO_FILE_PWD_}/bin/debug
} else {
    DESTDIR = $${_PRO_FILE_PWD_}/bin/release
}