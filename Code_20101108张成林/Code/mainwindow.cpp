#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QThread>
#include <QtDebug>
#include "process.h"
#include "imageprocess.h"
#include "qconvert.h"
#include "string.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QImage>
#include <QTimer>
using namespace cv;
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("图像处理工具");
    ui->debugEdit->append("20101108张成林的课程设计");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bgr2gray_button_clicked()
{
    imageprocess* gary = new imageprocess; //创建对象
    ui->debugEdit->append("创建算法对象");


    //接收属性参数

    QThread* mThread = new QThread;//创建线程
    gary->moveToThread(mThread);
    mThread->start();
    dstImage = gary->working();//运行算法
    ui->debugEdit->clear();
     ui->debugEdit->append("20101108ZhangChenglin");
    ui->debugEdit->append("20101108张成林的灰度化算法运行中");

    QImage img = cvMat2QImage(dstImage);//结果图转换为QImage
    ui->show_label->setPixmap(QPixmap::fromImage(img));//显示结果图

    delete gary;//删除对象


}

// lead_image 是某个菜单项对象名, 点击这个菜单项会读取图片
void MainWindow::on_lead_image_triggered()
{
    QString curDir=QDir::currentPath();//选择文件
    QString filename=QFileDialog::getOpenFileName(this,"select Image",curDir,
                                                      "Images (*.png *.bmp *.jpg *.tif *.GIF )");
    if(filename.isEmpty())
    {
         return;
    }
     QImage* img=new QImage;
     if(! ( img->load(filename) ) ) //加载图像
     {
         //qDebug()<<"打开图像失败";
         ui->debugEdit->append("打开图像失败");
         delete img;
         return;
     }

     ui->debugEdit->append("打开图像"+filename);
    *img = img->convertToFormat(QImage::Format_RGB32);
    srcImage = QImage2cvMat(*img);
    ui->show_label->setScaledContents(true);
    ui->show_label->setPixmap(QPixmap::fromImage(*img));
    delete img;
}

// save_image 是某个菜单项对象名, 点击这个菜单项会保存图片
void MainWindow::on_save_image_triggered()
{
    QString curDir=QDir::currentPath();//选择文件
    QString filename=QFileDialog::getSaveFileName(this,"save Image",curDir,
                                                      "Images (*.png *.bmp *.jpg *.tif *.GIF )");
    if(filename.isEmpty()||dstImage.empty())
    {
         return;
    }
    std::string save_path = filename.toLocal8Bit().toStdString();
    cv::imwrite(save_path,dstImage);//保存图片
    ui->debugEdit->append("保存图片成功");

}




void MainWindow::on_show_label_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)

    QMenu menu;//创建菜单
    menu.addAction(ui->lead_image);//添加菜单项
    menu.addAction(ui->save_image);
    menu.addAction(ui->statement);

    menu.exec(QCursor::pos()); //在鼠标位置显示菜单

}

void MainWindow::on_pushButton_5_clicked()
{
    imageprocess* gary = new imageprocess; //创建对象
    ui->debugEdit->append("创建算法对象");


    //接收属性参数

    QThread* mThread = new QThread;//创建线程
    gary->moveToThread(mThread);
    mThread->start();
    dstImage = gary->working();//运行算法
    ui->debugEdit->append("算法运行ing");
    namedWindow("Original Image",WINDOW_NORMAL);
    imshow("Original Image", dstImage);

    delete gary;//删除对象
    ui->debugEdit->append("算法结束");
}

void MainWindow::on_pushButton_4_clicked()
{
    imageprocess* gary = new imageprocess; //创建对象
    ui->debugEdit->append("创建算法对象");
    //接收属性参数

    QThread* mThread = new QThread;//创建线程
    gary->moveToThread(mThread);
    mThread->start();
    dstImage = gary->working();//运行算法
    int threshold_value = 127;
    int max_value = 255;
    createTrackbar("Threshold", "Binary Image", &threshold_value, max_value);

    namedWindow("Binary Image", WINDOW_NORMAL);

    Mat binary;
    threshold(dstImage, binary, threshold_value, max_value, THRESH_BINARY);
    imshow("Binary Image", binary);

    ui->debugEdit->append("算法运行ing");

    delete gary;//删除对象
    ui->debugEdit->append("算法结束");
}

void MainWindow::on_pushButton_6_clicked()
{


    ui->debugEdit->append("算法运行ing");

    namedWindow("median filter", WINDOW_NORMAL);
    // 使用OpenCV中的medianBlur函数进行3x3中值滤波
    cv::medianBlur(srcImage, dstImage, 3);
    // 将OpenCV的Mat转换为Qt中的QImage格式
    imshow("median filter",dstImage);
    ui->debugEdit->append("算法结束");
}

void MainWindow::on_pushButton_8_clicked()
{
    blur(srcImage, dstImage, Size(3, 3));
    namedWindow("average filter", WINDOW_NORMAL);//均值滤波
    imshow("average filter",dstImage);
}

void MainWindow::on_pushButton_9_clicked()
{//图片拉普拉斯锐化

    cv::cvtColor(srcImage, dstImage, cv::COLOR_BGR2GRAY);

    cv::Mat laplacianKernel = (cv::Mat_<float>(3, 3) << 0, 1, 0, 1, -4, 1, 0, 1, 0);

    cv::Mat laplacianImage;
    cv::filter2D(dstImage, laplacianImage, -1, laplacianKernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

    dstImage = dstImage - laplacianImage;
    namedWindow("Laplace sharpening", WINDOW_NORMAL);
    cv::imshow("Laplace sharpening", dstImage);

}

void MainWindow::on_pushButton_11_clicked()
{

    cv::cvtColor(srcImage, dstImage, cv::COLOR_BGR2GRAY);
    cv::Mat edges;
    cv::Canny(dstImage, dstImage, 50, 150);
    namedWindow("edge detection", WINDOW_NORMAL);
    cv::imshow("edge detection", dstImage);



}

void MainWindow::on_pushButton_10_clicked()
{
    int histSize = 256; // 直方图的尺寸
    float range[] = { 0, 256 }; // 像素值范围
    const float* histRange = { range };
    bool uniform = true, accumulate = false;
    cv::Mat hist;
    cv::cvtColor(srcImage, dstImage, cv::COLOR_BGR2GRAY);
    cv::calcHist(&dstImage, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);


    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    cv::Mat histImage(hist_h, hist_w, CV_8UC1, cv::Scalar(0, 0, 0));
    cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    for (int i = 1; i < histSize; i++)
    {
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
                 cv::Scalar(255, 255, 255), 2, 8, 0);
    }

    namedWindow("column diagram", WINDOW_NORMAL);
    namedWindow("Image", WINDOW_NORMAL);
    cv::imshow("Image", dstImage);
    cv::imshow("column diagram", histImage);



}

void MainWindow::on_pushButton_12_clicked()
{   // 打开视频文件
    cv::VideoCapture cap("D:\\Qt_Demo\\untitled3\\video.mp4");

    // 检查是否打开成功
    if(!cap.isOpened())
    {
        qDebug() << "打开视频失败";
        return ;
    }
    // 循环读取视频中的每一帧图像
    Mat frame;
    while(cap.read(frame))
    {
        // 将图像转换为灰度图像
        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        // 将灰度图像转换为Qt的图片格式
        QImage img(gray.data, gray.cols, gray.rows, gray.step, QImage::Format_Grayscale8);

        // 在标签上显示图像
        ui->show_label->setScaledContents(true);
        ui->show_label->setPixmap(QPixmap::fromImage(img));

        // 等待33毫秒，以控制视频播放速度
        waitKey(33);
    }

}

void MainWindow::on_pushButton_13_clicked()
{
    cv::VideoCapture cap("D:\\Qt_Demo\\untitled3\\video.mp4");
    // 打开视频文件

      // 检查是否打开成功
      if(!cap.isOpened())
      {
          ui->debugEdit->append("视频打开失败");
          return ;
      }
    //接收属性参数
      Mat frame;
      while(cap.read(frame))
      {
          // 将图像转换为灰度图像
          Mat gray;
          cvtColor(frame, gray, COLOR_BGR2GRAY);
          int threshold_value = 127;
              int max_value = 255;

              Mat binary;
              threshold(gray, binary, threshold_value, max_value, THRESH_BINARY);

          // 将灰度图像转换为Qt的图片格式
          QImage img(binary.data, binary.cols, binary.rows, binary.step, QImage::Format_Grayscale8);

          // 在标签上显示图像
          ui->show_label->setScaledContents(true);
          ui->show_label->setPixmap(QPixmap::fromImage(img));

          // 等待33毫秒，以控制视频播放速度
          waitKey(33);
      }
}

void MainWindow::on_pushButton_14_clicked()
{
    cv::VideoCapture cap("D:\\Qt_Demo\\untitled3\\video.mp4");
    // 打开视频文件

      // 检查是否打开成功
      if(!cap.isOpened())
      {
          ui->debugEdit->append("视频打开失败");
          return ;
      }

    //接收属性参数
      Mat frame;
      while(cap.read(frame))
      {
          // 将图像转换为灰度图像
          Mat gray;
         // cvtColor(frame, gray, COLOR_BGR2GRAY);
          //均值滤波
          blur(frame, frame, Size(3, 3));



          // 将灰度图像转换为Qt的图片格式
          QImage img(frame.data, frame.cols, frame.rows, frame.step,QImage::Format_RGB888);

          // 在标签上显示图像
          ui->show_label->setScaledContents(true);
          ui->show_label->setPixmap(QPixmap::fromImage(img));

          // 等待33毫秒，以控制视频播放速度
          waitKey(33);
      }
}

void MainWindow::on_pushButton_15_clicked()
{
    cv::VideoCapture cap("D:\\Qt_Demo\\untitled3\\video.mp4");
    // 打开视频文件

      // 检查是否打开成功
      if(!cap.isOpened())
      {
          ui->debugEdit->append("视频打开失败");
          return ;
      }
    //接收属性参数
      Mat frame;
      while(cap.read(frame))
      {

          // 使用OpenCV中的medianBlur函数进行3x3中值滤波
          cv::medianBlur(frame, frame, 3);
          // 将灰度图像转换为Qt的图片格式
          QImage img(frame.data, frame.cols, frame.rows, frame.step,QImage::Format_RGB888);

          // 在标签上显示图像
          ui->show_label->setScaledContents(true);
          ui->show_label->setPixmap(QPixmap::fromImage(img));

          // 等待33毫秒，以控制视频播放速度
          waitKey(33);
      }
}

void MainWindow::on_algoButton_1_clicked()
{
    cv::VideoCapture cap("D:\\Qt_Demo\\untitled3\\video.mp4");
    // 打开视频文件
      // 检查是否打开成功
      if(!cap.isOpened())
      {
          ui->debugEdit->append("视频打开失败");
          return ;
      }
    //接收属性参数
      Mat frame;
      while(cap.read(frame))
      {
          // 将图像转换为灰度图像
          Mat gray;
          cvtColor(frame, gray, COLOR_BGR2GRAY);

          //构建拉普拉斯算子
          cv::Mat laplacianKernel = (cv::Mat_<float>(3, 3) << 0, 1, 0, 1, -4, 1, 0, 1, 0);
          cv::Mat laplacianImage;
          cv::filter2D(gray, laplacianImage, -1, laplacianKernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
          gray = gray - laplacianImage;

          // 将灰度图像转换为Qt的图片格式
          QImage img(gray.data, gray.cols, gray.rows, gray.step,QImage::Format_Grayscale8);

          // 在标签上显示图像
          ui->show_label->setScaledContents(true);
          ui->show_label->setPixmap(QPixmap::fromImage(img));

          // 等待40毫秒，以控制视频播放速度
          waitKey(40);
      }
}

void MainWindow::on_pushButton_clicked()
{
// 转换到 HIS 颜色空间
Mat hisImg;
cvtColor(srcImage, hisImg, COLOR_BGR2HLS);

// 分离 H、I、S 通道
std::vector<Mat> channels;
split(hisImg, channels);

// 对亮度进行灰度增强，调整对比度
channels[1].convertTo(channels[1], CV_32F);
channels[1] = channels[1] * 1.2;
normalize(channels[1], channels[1], 0, 255, NORM_MINMAX);
channels[1].convertTo(channels[1], CV_8UC1);

// 合并 H、I、S 通道
merge(channels, hisImg);

// 转换回 RGB 颜色空间
Mat dstImg;
cvtColor(hisImg, dstImg, COLOR_HLS2BGR);

// 显示处理前后的图像
namedWindow("before process", WINDOW_NORMAL);
imshow("before process", srcImage);

namedWindow("after process", WINDOW_NORMAL);
imshow("after process", dstImg);
dstImage=dstImg;


}






