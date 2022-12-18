#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <fstream>

const QString VERSION = APP_VERSION;

// Constants.
const float INPUT_WIDTH = 640.0;
const float INPUT_HEIGHT = 640.0;
const float SCORE_THRESHOLD = 0.5;
const float NMS_THRESHOLD = 0.45;
const float CONFIDENCE_THRESHOLD = 0.65;

// Text parameters.
const float FONT_SCALE = 0.7;
const int FONT_FACE = FONT_HERSHEY_SIMPLEX;
const int THICKNESS = 1;

// Colors.
Scalar BLACK = Scalar(0, 0, 0);
Scalar BLUE = Scalar(255, 178, 50);
Scalar YELLOW = Scalar(0, 255, 255);
Scalar RED = Scalar(0, 0, 255);

// Draw the predicted bounding box.
void draw_label(Mat &input_image, string label, int left, int top)
{
    // Display the label at the top of the bounding box.
    int baseLine;
    Size label_size = getTextSize(label, FONT_FACE, FONT_SCALE, THICKNESS, &baseLine);
    top = max(top, label_size.height);
    // Top left corner.
    Point tlc = Point(left, top);
    // Bottom right corner.
    Point brc = Point(left + label_size.width, top + label_size.height + baseLine);
    // Draw black rectangle.
    rectangle(input_image, tlc, brc, BLACK, FILLED);
    // Put the label on the black rectangle.
    putText(input_image, label, Point(left, top + label_size.height), FONT_FACE, FONT_SCALE, YELLOW, THICKNESS);
}

vector<Mat> pre_process(Mat &input_image, Net &net)
{
    // Convert to blob.
    Mat blob;
    blobFromImage(input_image, blob, 1. / 255., Size(INPUT_WIDTH, INPUT_HEIGHT), Scalar(), true, false);

    net.setInput(blob);

    // Forward propagate.
    vector<Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    return outputs;
}

Mat post_process(Mat &input_image, vector<Mat> &outputs, const vector<string> &class_name)
{
    // Initialize vectors to hold respective outputs while unwrapping detections.
    vector<int> class_ids;
    vector<float> confidences;
    vector<Rect> boxes;

    // Resizing factor.
    float x_factor = input_image.cols / INPUT_WIDTH;
    float y_factor = input_image.rows / INPUT_HEIGHT;

    float *data = (float *)outputs[0].data;

    const int dimensions = 85;
    const int rows = 25200;
    // Iterate through 25200 detections.
    for (int i = 0; i < rows; ++i)
    {
        float confidence = data[4];
        // Discard bad detections and continue.
        if (confidence >= CONFIDENCE_THRESHOLD)
        {
            float *classes_scores = data + 5;
            // Create a 1x85 Mat and store class scores of 80 classes.
            Mat scores(1, class_name.size(), CV_32FC1, classes_scores);
            // Perform minMaxLoc and acquire index of best class score.
            Point class_id;
            double max_class_score;
            minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
            // Continue if the class score is above the threshold.
            if (max_class_score > SCORE_THRESHOLD)
            {
                // Store class ID and confidence in the pre-defined respective vectors.

                confidences.push_back(confidence);
                class_ids.push_back(class_id.x);

                // Center.
                float cx = data[0];
                float cy = data[1];
                // Box dimension.
                float w = data[2];
                float h = data[3];
                // Bounding box coordinates.
                int left = int((cx - 0.5 * w) * x_factor);
                int top = int((cy - 0.5 * h) * y_factor);
                int width = int(w * x_factor);
                int height = int(h * y_factor);
                // Store good detections in the boxes vector.
                boxes.push_back(Rect(left, top, width, height));
            }
        }
        // Jump to the next column.
        data += 85;
    }

    // Perform Non Maximum Suppression and draw predictions.
    vector<int> indices;
    NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);
    for (int i = 0; i < indices.size(); i++)
    {
        int idx = indices[i];
        Rect box = boxes[idx];

        int left = box.x;
        int top = box.y;
        int width = box.width;
        int height = box.height;
        // Draw bounding box.
        rectangle(input_image, Point(left, top), Point(left + width, top + height), BLUE, 3 * THICKNESS);

        // Get the label for the class name and its confidence.
        string label = format("%.2f", confidences[idx]);
        label = class_name[class_ids[idx]] + ":" + label;
        // Draw class labels.
        draw_label(input_image, label, left, top);
    }
    return input_image;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      timer(new QTimer)
{
    ui->setupUi(this);
    this->setWindowTitle("HelloQT " + VERSION);

    connect(timer, &QTimer::timeout, this, &MainWindow::updateShow);

    QFile file(modelPath);
    if (file.exists())
    {
        qInfo() << modelPath;
        isClassifierLoaded = classifier.load(modelPath.toStdString()); //加载训练文件
    }
    else
    {
        ui->label_result->setText(QString("请设置模型文件目录"));
        isClassifierLoaded = false;
    }

    ifstream ifs(names_path.toStdString());
    string line;

    while (getline(ifs, line))
    {
        class_list.push_back(line);
    }

    on_openButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateShow()
{
    cap >> frame;
    imageShow(frame, ui->label_video);

    timer->start(200);
}

void MainWindow::on_openButton_clicked()
{
    cap.open(0);

    timer->start(200);
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
    ui->openButton->setText(QString("继续"));
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

    Mat out_img = process_image(img);

    if (isClassifierLoaded) //加载训练文件已成功
    {
        cv::Mat gray;

        cvtColor(out_img, gray, cv::COLOR_BGR2GRAY); //转换成灰度图，因为harr特征从灰度图中提取

        equalizeHist(gray, gray); //直方图均衡行

        classifier.detectMultiScale(gray,
                                    faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE,
                                    cv::Size(30, 30)); //检测人脸

        //画方框
        for (int i = 0; i < (int)faces.size(); i++)
        {
            cv::Point centera(faces[i].x, faces[i].y);
            cv::Point centerb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
            rectangle(out_img, centera, centerb, cv::Scalar(255, 0, 0));
        }
    }

    image = mat2QImage(out_img);

    // image = image.mirrored(true, false); //将图像水平镜像
    image = image.scaled(label->size(),
                         Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);
    label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_action_model_triggered()
{
    QString srcDir = QFileDialog::getExistingDirectory(this, QString("选择模型目录"), modelDir);
    if (srcDir.isEmpty())
    {
        return;
    }
    else
    {
        qDebug() << "模型目录:" << srcDir;
        modelPath = srcDir + QString("/face2.xml");
        qDebug() << modelPath;
        isClassifierLoaded = classifier.load(modelPath.toStdString()); //加载训练文件
    }
}

Mat MainWindow::process_image(Mat img)
{
    vector<Mat> detections;
    detections = pre_process(img, net);
    Mat out_img = post_process(img, detections, class_list);
    vector<double> layersTimes;
    double freq = getTickFrequency() / 1000;
    double t = net.getPerfProfile(layersTimes) / freq;
    string label = format("Inference time : %.2f ms", t);
    putText(out_img, label, Point(20, 40), FONT_FACE, FONT_SCALE, RED);

    return out_img;
}
