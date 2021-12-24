#include "channelsfilter.h"

ChannelsFilter::ChannelsFilter()
{
}

ChannelsFilter::~ChannelsFilter()
{
}

void ChannelsFilter::process(cv::Mat &inMat, cv::Mat &outMat)
{
    cv::Mat finalMask;
    for (auto f : filters) {
        cv::Mat mask;
        cv::inRange(inMat, cv::Scalar(f._min1, f._min2, f._min3), cv::Scalar(f._max1, f._max2, f._max3), mask);
        if (finalMask.empty()) {
            finalMask = mask;
        }
        else {
            if (f._mode == 0) {
                cv::bitwise_and(finalMask, mask, finalMask);
            }
            else {
                cv::bitwise_or(finalMask, mask, finalMask);
            }
        }
    }

    cv::Mat outBlank(inMat.size(), inMat.type(), cv::Scalar(0));
    inMat.copyTo(outBlank, finalMask);
    outMat = outBlank;
}

void ChannelsFilter::add(int min1, int min2, int min3, int max1, int max2, int max3, int mode)
{
    filters.push_back(Filter(min1, min2, min3, max1, max2, max3, mode));
}

}
