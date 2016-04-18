#ifndef IPMETHOD_H
#define IPMETHOD_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>

#include <QString>

class ipmethod
{
public:
    ipmethod();
    ~ipmethod();

    virtual void process(cv::Mat&, cv::Mat&) = 0;
};

#endif // IPMETHOD_H
