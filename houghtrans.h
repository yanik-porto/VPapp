#ifndef HOUGHTRANS_H
#define HOUGHTRANS_H

#include "ipmethod.h"

class HoughTrans : public ipmethod
{
private:
    QString shape;
    int thresh;

public:
    HoughTrans();
    HoughTrans(const QString &, const int &);
    HoughTrans(const QString &);
    HoughTrans(const HoughTrans &);
    ~HoughTrans();


    void set_shape(const QString &);
    void set_thresh(const int &);
    const QString &get_shape() const {return shape;}
    const int & get_thresh() const {return thresh;}

    void findLines(cv::Mat &, cv::Mat &, int &);
    void findCircles(cv::Mat &, cv::Mat &, int &);
    void findContour(cv::Mat &, cv::Mat &);

    void process(cv::Mat &, cv::Mat &);


};

#endif // HOUGHTRANS_H
