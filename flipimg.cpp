#include "flipimg.h"

flipImg::flipImg()
{

}

flipImg::flipImg(const int &n)
{
    flipCode = n;
}

flipImg::flipImg(const flipImg &flImg)
{
    flipCode = flImg.get_flipcode();
}

flipImg::~flipImg()
{

}

void flipImg::process(cv::Mat &inMat, cv::Mat &outMat)
{
    cv::flip( inMat, outMat, flipCode );
}

void flipImg::set_flipcode(const int &n)
{
    flipCode = n;
}

