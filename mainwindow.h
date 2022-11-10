#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCamera>
#include <QImageCapture>
#include <QMainWindow>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QMediaRecorder>
#include <QVideoWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QScopedPointer<QCamera> my_camera;
    QScopedPointer<QMediaRecorder> my_mediaRecorder;
    QMediaCaptureSession my_captureSession;

    bool checkCameraAvailability(void);
};
#endif // MAINWINDOW_H
