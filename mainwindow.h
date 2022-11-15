#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QTimer>

#include "opencv2/objdetect.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>

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

    void imageShow(const cv::Mat &img, QLabel *label);
    void updateShow();

private slots:
    void on_openButton_clicked();

    void on_detectButton_clicked();

private:
    Ui::MainWindow *ui;

    QTimer *timer;
    cv::Mat frame;
    QImage image;
    cv::VideoCapture cap;

    cv::CascadeClassifier classifier; //创建分类器对象
    bool isClassifierLoaded;          //分类器加载成功标识
    std::vector<cv::Rect> faces;
};
#endif // MAINWINDOW_H
