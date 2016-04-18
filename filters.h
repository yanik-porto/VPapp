#ifndef FILTERS_H
#define FILTERS_H

#include "ipmethod.h"

class Filters : public ipmethod
{

private:
    int sizeKernel;
    QString method;

public:
    Filters();
    Filters(const QString &, const int &);
    Filters(const Filters &);
    ~Filters();

    void set_sizeKernel(const int &);
    void set_method(const QString &);

    void erode( cv::Mat inMat, cv::Mat outMat, int sz );
    void dilate( cv::Mat inMat, cv::Mat outMat, int sz );
    void open( cv::Mat inMat, cv::Mat outMat, int sz );
    void close( cv::Mat inMat, cv::Mat outMat, int sz );
    void sharpen( cv::Mat inMat, cv::Mat outMat, int center );
    void blur( cv::Mat inMat, cv::Mat outMat, int sz);

    void process(cv::Mat &, cv::Mat &);
};

#endif // FILTERS_H
