#ifndef COLORBASICS_H
#define COLORBASICS_H

#include "ipmethod.h"

class colorBasics : public ipmethod
{
private:
    QString method;

public:
    colorBasics();
    colorBasics( const QString &);
    colorBasics( const colorBasics &);
    ~colorBasics();

    void set_method(const QString &);
    const QString &get_method() const {return method;}

    void invert( cv::Mat &, cv::Mat &);
    void equalize( cv::Mat &, cv::Mat &);

    void process(cv::Mat &, cv::Mat &);
};

#endif // COLORBASICS_H
