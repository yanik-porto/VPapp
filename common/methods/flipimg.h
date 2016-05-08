#ifndef FLIPIMG_H
#define FLIPIMG_H

#include "ipmethod.h"

class flipImg : public ipmethod
{
private:
    int flipCode;

public:
    flipImg();
    flipImg(const int &);
    flipImg(const flipImg &);
    ~flipImg();

    void set_flipcode(const int &);
    const int &get_flipcode() const {return flipCode;}

    void process(cv::Mat &, cv::Mat &);
};

#endif // FLIPIMG_H
