#include "resizeimg.h"

resizeImg::resizeImg()
{

}

resizeImg::resizeImg(const int &w, const int &h)
{
    width = w;
    height = h;
}

resizeImg::resizeImg(const resizeImg &rszImg)
{
    width = rszImg.get_width();
    height= rszImg.get_height();
}

resizeImg::~resizeImg()
{

}

void resizeImg::process(cv::Mat &inMat, cv::Mat &outMat)
{
    cv::Size size( width, height );
    cv::resize( inMat, outMat, size );
}

QString resizeImg::toStr()
{
    return "resize image";
}

void resizeImg::set_width(const int &w)
{
    width = w;
}

void resizeImg::set_height(const int &h)
{
    height = h;
}

