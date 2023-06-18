#ifndef QCONVERT_H
#define QCONVERT_H



#include <QtCore/QDebug>
#include <QtGui/QImage>
#include "process.h"

/**
 * @brief 将 OpenCV 的 cv::Mat 类型图像转换为 QImage 类型

*/
QImage cvMat2QImage(const cv::Mat& mat, bool clone = true, bool rb_swap = true);

/**
 * @brief 将 QImage 的类型图像转换为 cv::Mat 类型

*/
cv::Mat QImage2cvMat(QImage &image, bool clone = false, bool rb_swap = true);



#endif // QCONVERT_H
