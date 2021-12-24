#ifndef DERIVATIVES_H
#define DERIVATIVES_H

#include "ipmethod.h"

class Derivatives : public ipmethod
{
private :
    QString method;

public:
    Derivatives();
    Derivatives( const QString & );
    Derivatives( const Derivatives & );
    ~Derivatives();

    void process( cv::Mat &, cv::Mat & );

    void set_method( const QString & );

    void sobel(cv::Mat &, cv::Mat & );
    void laplacian( cv::Mat &, cv::Mat &);
    void canny( cv::Mat &, cv::Mat &);

    QString toStr() override;

};

#endif // DERIVATIVES_H
