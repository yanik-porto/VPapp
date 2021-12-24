#ifndef CHANNELSFILTER_H
#define CHANNELSFILTER_H

#include "ipmethod.h"

class ChannelsFilter : public ipmethod
{
public:
    ChannelsFilter();
    virtual ~ChannelsFilter();

    void process(cv::Mat &, cv::Mat &) override;

    void add(int min1, int min2, int min3, int max1, int max2, int max3, int mode);
private:
    struct Filter {
        Filter(int min1, int min2, int min3, int max1, int max2, int max3, int mode) {
            _min1 = min1;
            _min2 = min2;
            _min3 = min3;
            _max1 = max1;
            _max2 = max2;
            _max3 = max3;
            _mode = mode;
        }
        int _min1;
        int _min2;
        int _min3;
        int _max1;
        int _max2;
        int _max3;
        int _mode;
    };

    std::vector<Filter> filters;
};

#endif // CHANNELSFILTER_H
