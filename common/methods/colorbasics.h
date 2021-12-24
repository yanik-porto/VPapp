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
    void convertToRGB( cv::Mat &, cv::Mat & );
    void convertToHSV( cv::Mat &, cv::Mat & );
    void convertToGRAY( cv::Mat &, cv::Mat & );

    void process(cv::Mat &, cv::Mat &) override;

    QString toStr() override;
};

#endif // COLORBASICS_H
