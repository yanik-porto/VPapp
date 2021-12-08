#ifndef CHANNELSFILTER_H
#define CHANNELSFILTER_H

#include "ipmethod.h"

class ChannelsFilter : public ipmethod
{
public:
    ChannelsFilter();
    ChannelsFilter(int min1, int min2, int min3, int max1, int max2, int max3);
    ~ChannelsFilter();

    void process(cv::Mat &, cv::Mat &) override;

private:
    int _min1;
    int _min2;
    int _min3;
    int _max1;
    int _max2;
    int _max3;
};

#endif // CHANNELSFILTER_H
