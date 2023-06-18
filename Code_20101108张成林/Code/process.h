#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

extern cv::Mat srcImage;
extern cv::Mat dstImage;

class Process : public QObject
{
    Q_OBJECT
public:
//    static cv::Mat srcImage;
//    static cv::Mat dstImage;//定义静态变量src,dst来存储输入图像和输出图像

public:
    explicit Process(QObject *parent = nullptr);
    //void process_init(Process c);
    virtual cv::Mat working()=0;//虚函数接口

signals:

public slots:
};

#endif // PROCESS_H
