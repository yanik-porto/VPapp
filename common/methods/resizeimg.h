#ifndef RESIZEIMG_H
#define RESIZEIMG_H

#include "ipmethod.h"

class resizeImg : public ipmethod
{
private:
    int width;
    int height;

public:
    resizeImg();
    resizeImg(const int &, const int &);
    resizeImg(const resizeImg &);
    ~resizeImg();

    void set_width(const int &);
    void set_height(const int &);
    const int &get_width() const {return width;}
    const int &get_height() const {return height;}

    void process(cv::Mat &, cv::Mat & );

    QString toStr() override;

};

#endif // RESIZEIMG_H
