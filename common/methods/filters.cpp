#include "filters.h"

Filters::Filters()
{
    method = QString("blur");
    sizeKernel = 11;
}

Filters::Filters(const QString &str, const int &sz)
{
    method = str;
    sizeKernel = sz;
}

Filters::Filters(const Filters &filt)
{
    set_method(filt.method);
    set_sizeKernel(filt.sizeKernel);
}

Filters::~Filters()
{

}

void Filters::process(cv::Mat &inMat, cv::Mat &outMat)
{
    if( method.compare("blur") ==0 )
    {
        blur( inMat, outMat, sizeKernel );
    }

    if( method.compare("sharpen") ==0 )
    {
        sharpen( inMat, outMat, sizeKernel );
    }

    if( method.compare("erode") ==0 )
    {
        erode( inMat, outMat, sizeKernel );
    }

    if( method.compare("dilate") ==0 )
    {
        dilate( inMat, outMat, sizeKernel );
    }

    if( method.compare("open") ==0 )
    {
        open( inMat, outMat, sizeKernel );
    }

    if( method.compare("close") ==0 )
    {
        close( inMat, outMat, sizeKernel );
    }
}

void Filters::set_sizeKernel(const int &sz)
{
    sizeKernel = sz;
}

void Filters::set_method(const QString &str)
{
    method = str;
}

void Filters::erode(cv::Mat inMat, cv::Mat outMat, int sz)
{
    cv::Mat	element( sz, sz, CV_8U,cv::Scalar(1) );
    cv::erode( inMat, outMat, element );
}

void Filters::dilate(cv::Mat inMat, cv::Mat outMat, int sz)
{
    cv::Mat	element( sz, sz, CV_8U,cv::Scalar(1) );
    cv::dilate( inMat, outMat, element );
}

void Filters::open(cv::Mat inMat, cv::Mat outMat, int sz)
{
    cv::Mat	element( sz, sz, CV_8U,cv::Scalar(1) );
    cv::morphologyEx( inMat, outMat , cv::MORPH_OPEN, element );
}

void Filters::close(cv::Mat inMat, cv::Mat outMat, int sz)
{
    cv::Mat	element( sz, sz, CV_8U,cv::Scalar(1) );
    cv::morphologyEx( inMat, outMat , cv::MORPH_CLOSE, element );
}

void Filters::sharpen(cv::Mat inMat, cv::Mat outMat, int center)
{
    float border = -(center-1)/4;
    int kernel_size = 3;
    cv::Mat kernel = ( cv::Mat_<double>( kernel_size, kernel_size ) << 0, border, 0, border, center, border, 0, border, 0 );
    cv::filter2D( inMat, outMat, -1, kernel );
}

void Filters::blur(cv::Mat inMat, cv::Mat outMat, int sz)
{
    cv::blur( inMat, outMat, cv::Size( sz, sz ) );
}

