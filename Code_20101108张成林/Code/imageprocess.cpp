#include "imageprocess.h"
#include "process.h"
imageprocess::imageprocess()
{

}

 cv::Mat imageprocess::working()
{
    cv::cvtColor(srcImage,dstImage,cv::COLOR_BGR2GRAY);
    return dstImage;
}
