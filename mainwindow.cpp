#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    my_camera.reset(new QCamera(QMediaDevices::defaultVideoInput())); //新建并设置摄像头使用默认驱动
    my_captureSession.setCamera(my_camera.data());                    //捕获摄像头画面
    my_captureSession.setVideoOutput(ui->viewfinder);                 //设置捕捉画面显示窗口
    my_camera->start();                                               //启动摄像头
}

MainWindow::~MainWindow()
{
    delete ui;
}

//检测摄像头驱动能否识别
bool MainWindow::checkCameraAvailability()
{
    if (QMediaDevices::videoInputs().count() > 0)
        return true; //检测到摄像头驱动
    else
        return false; //没有检测到摄像头驱动
}
