#include "channelsfilter.h"

ChannelsFilter::ChannelsFilter()
{
}

ChannelsFilter::ChannelsFilter(int min1, int min2, int min3, int max1, int max2, int max3)
{
    _min1 = min1;
    _min2 = min2;
    _min3 = min3;
    _max1 = max1;
    _max2 = max2;
    _max3 = max3;
}

ChannelsFilter::~ChannelsFilter()
{
}

void ChannelsFilter::process(cv::Mat &inMat, cv::Mat &outMat)
{
    cv::Mat mask;
    cv::inRange(inMat, cv::Scalar(_min1, _min2, _min3), cv::Scalar(_max1, _max2, _max3), mask);
    cv::cvtColor(mask,mask,cv::COLOR_GRAY2BGR);
    cv::bitwise_and(inMat, mask, outMat);

//    cv::Mat image = inMat;
//    for(int y=0;y<image.rows;y++) {
//        for(int x=0;x<image.cols;x++){
//            cv::Vec3b &color = image.at<cv::Vec3b>(y, x);
//            if (color[0] <= _max1) {
//                color[0] = 0;
//            }
//            if (color[1] <= _max2) {
//                color[1] = 0;
//            }
//            if (color[2] <= _max3) {
//                color[2] = 0;
//            }
//        }
//    }
//    outMat = image;

}
