#ifndef FEATURES_H
#define FEATURES_H

#include "ipmethod.h"

using namespace cv::xfeatures2d;

class FeaturesPts : public ipmethod
{
private:
    QString method;
    int thresh;

public:
    FeaturesPts();
    FeaturesPts(const QString &, int &);
    FeaturesPts(const FeaturesPts &);
    ~FeaturesPts();

    void set_method(const QString &);
    void set_thresh(const int &);
    const QString &get_method() const {return method;}
    const int &get_thresh() const {return thresh;}

    void process(cv::Mat &, cv::Mat &);

    void Harris_keypts(cv::Mat &, cv::Mat &, const int &);
    void SURF_keypts(cv::Mat &, cv::Mat &, const int &);
    void SIFT_keypts(cv::Mat &, cv::Mat &, const int &);
    void FAST_keypts(cv::Mat &, cv::Mat &, const int &);
};

#endif // FEATURES_H
