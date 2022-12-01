#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <QTimer>

#include "opencv2/objdetect.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;
using namespace cv::dnn;

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
    Mat process_image(cv::Mat img);

private slots:
    void on_openButton_clicked();

    void on_detectButton_clicked();

    void on_action_model_triggered();

private:
    Ui::MainWindow *ui;

    // TODO: 自动获取模型文件存储路径
    QString modelDir = "/Users/liuwen/Documents/work/repos/github.com/iminders/helloqt/models/";
    QString modelPath = modelDir + QString("face2.xml");
    QTimer *timer;
    cv::Mat frame;
    QImage image;
    cv::VideoCapture cap;

    cv::CascadeClassifier classifier; //创建分类器对象
    bool isClassifierLoaded;          //分类器加载成功标识
    std::vector<cv::Rect> faces;

    vector<string> class_list;

    QString names_path = modelDir + QString("coco.names");
    QString model_path = modelDir + QString("yolov5s.onnx");
    dnn::Net net = dnn::readNet(model_path.toStdString());
};
#endif // MAINWINDOW_H
