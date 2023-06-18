#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include "process.h"

class imageprocess : public Process
{
public:
    imageprocess();
    virtual cv::Mat working();//灰度化函数，返回Mat类型的dstImage（全局
};

#endif // IMAGEPROCESS_H
