QT       += core gui
QT       += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APP_VERSION="\\\"V$${HELLOQT_VERSION}\\\""
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


# 输出配置
build_type =
CONFIG(debug, debug|release) {
    build_type = build_debug
} else {
    build_type = build_release
}

DESTDIR     = $$build_type/out
OBJECTS_DIR = $$build_type/obj
MOC_DIR     = $$build_type/moc
RCC_DIR     = $$build_type/rcc
UI_DIR      = $$build_type/ui

# 平台配置
win32:{
#    INCLUDEPATH += $${OPENCV_DIR}\include\opencv2 \
#                    $${OPENCV_DIR}\include

#    LIBS += $${OPENCV_DIR}\x64\mingw\lib\libopencv_*.a

    VERSION = $${HELLOQT_VERSION}.000
    RC_ICONS = "img\ico.ico"
    QMAKE_TARGET_PRODUCT = "helloqt"
    QMAKE_TARGET_DESCRIPTION = "helloqt based on Qt $$[QT_VERSION]"
    QMAKE_TARGET_COPYRIGHT = "GNU General Public License v3.0"

    git_tag.commands = $$quote("cd $$PWD && git describe --always --long --dirty --abbrev=10 --tags | $$PWD/tools/awk/awk.exe \'{print \"\\\"\"\$$0\"\\\"\"}\' > git_tag.inc")
}

unix:!macx:{
    QMAKE_RPATHDIR=$ORIGIN
    QMAKE_LFLAGS += -no-pie

    INCLUDEPATH += -I $${OPENCV_DIR}/include/opencv4
    DEPENDPATH +=$${OPENCV_DIR}/include/opencv4

    INCLUDEPATH += -I $${OPENCV_DIR}/include
    DEPENDPATH +=$${OPENCV_DIR}/include

    LIBS += -L $${OPENCV_DIR}/lib/ -lopencv_imgproc
    LIBS += -L $${OPENCV_DIR}/lib/ -lopencv_core

    git_tag.commands = $$quote("cd $$PWD && git describe --always --long --dirty --abbrev=10 --tags | awk \'{print \"\\\"\"\$$0\"\\\"\"}\' > git_tag.inc")
}

macx:{
    QMAKE_RPATHDIR=$ORIGIN
    ICON = "img/ico.icns"
    QMAKE_INFO_PLIST= macos/Info.plist

#    INCLUDEPATH += -I $${OPENCV_DIR}/include/opencv4
#    DEPENDPATH +=$${OPENCV_DIR}/include/opencv4

#    INCLUDEPATH += -I $${OPENCV_DIR}/include
#    DEPENDPATH +=$${OPENCV_DIR}/include

#    LIBS += -L $${OPENCV_DIR}/lib/ -lopencv_imgproc
#    LIBS += -L $${OPENCV_DIR}/lib/ -lopencv_core

    git_tag.commands = $$quote("cd $$PWD && git describe --always --long --dirty --abbrev=10 --tags | awk \'{print \"\\\"\"\$$0\"\\\"\"}\' > git_tag.inc")

}

git_tag.target = $$PWD/git_tag.inc
git_tag.depends = FORCE
PRE_TARGETDEPS += $$PWD/git_tag.inc
QMAKE_EXTRA_TARGETS += git_tag
