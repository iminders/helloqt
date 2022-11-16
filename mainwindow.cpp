#include "mainwindow.h"
#include "ui_mainwindow.h"
const QString VERSION = APP_VERSION;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      timer(new QTimer)
{
    ui->setupUi(this);
    this->setWindowTitle("HelloQT " + VERSION);

    connect(timer, &QTimer::timeout, this, &MainWindow::updateShow);

    QFile file(modelPath);
    if (file.exists()) {
        qInfo() << modelPath;
        isClassifierLoaded = classifier.load(modelPath.toStdString()); //加载训练文件
    }
    else {
        ui->label_result->setText(QString("请设置模型文件目录"));
        isClassifierLoaded = false;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateShow()
{
    cap >> frame;
    imageShow(frame, ui->label_video);

    timer->start(50);
}

void MainWindow::on_openButton_clicked()
{
    cap.open(0);

    timer->start(50);
}

void MainWindow::on_detectButton_clicked()
{
    timer->stop();
    cap.release();

    QString tmp = "";
    for (int i = 0; i < (int)faces.size(); i++)
    {
        tmp.append(QString("Face:") +
                   QString::number(i).leftJustified(4, ' ') +
                   QString(" x=") + QString::number(faces[i].x, 'f', 1).rightJustified(4, ' ') +
                   QString(", y=") + QString::number(faces[i].y, 'f', 1).rightJustified(4, ' ') +
                   QString(", width=") + QString::number(faces[i].width, 'f', 1).rightJustified(4, ' ') +
                   QString(", height=") + QString::number(faces[i].height, 'f', 1).rightJustified(4, ' ') +
                   QString("\n"));
    }
    ui->label_result->setText(tmp);
}

QImage mat2QImage(cv::Mat cvImg)
{
    QImage qImg;
    if (cvImg.channels() == 3) // 3 channels color image
    {

        cv::cvtColor(cvImg, cvImg, cv::COLOR_BGR2RGB);
        qImg = QImage((const unsigned char *)(cvImg.data),
                      cvImg.cols, cvImg.rows,
                      cvImg.cols * cvImg.channels(),
                      QImage::Format_RGB888);
    }
    else if (cvImg.channels() == 1) // grayscale image
    {
        qImg = QImage((const unsigned char *)(cvImg.data),
                      cvImg.cols, cvImg.rows,
                      cvImg.cols * cvImg.channels(),
                      QImage::Format_Indexed8);
    }
    else
    {
        qImg = QImage((const unsigned char *)(cvImg.data),
                      cvImg.cols, cvImg.rows,
                      cvImg.cols * cvImg.channels(),
                      QImage::Format_RGB888);
    }

    return qImg;
}

void MainWindow::imageShow(const cv::Mat &img, QLabel *label)
{
    if (img.empty())
        return;
    if (isClassifierLoaded) //加载训练文件已成功
    {
        cv::Mat gray;

        cvtColor(img, gray, cv::COLOR_BGR2GRAY); //转换成灰度图，因为harr特征从灰度图中提取

        equalizeHist(gray, gray); //直方图均衡行

        classifier.detectMultiScale(gray,
                                    faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE,
                                    cv::Size(30, 30)); //检测人脸

        //画方框
        for (int i = 0; i < (int)faces.size(); i++)
        {
            cv::Point centera(faces[i].x, faces[i].y);
            cv::Point centerb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
            rectangle(img, centera, centerb, cv::Scalar(255, 0, 0));
        }
    }

    image = mat2QImage(img);
    image = image.mirrored(true, false); //将图像水平镜像
    image = image.scaled(label->size(),
                         Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);
    label->setPixmap(QPixmap::fromImage(image));
}



void MainWindow::on_action_model_triggered()
{
    QString srcDir = QFileDialog::getExistingDirectory(this, QString("选择模型目录"), modelDir);
    if (srcDir.isEmpty()){
        return;
    }
    else{
        qDebug() << "模型目录:" << srcDir;
        modelPath = srcDir + QString("/face2.xml");
        qDebug() << modelPath;
        isClassifierLoaded = classifier.load(modelPath.toStdString()); //加载训练文件
    }
}

