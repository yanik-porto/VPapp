#include "colorbasics.h"

colorBasics::colorBasics()
{

}

colorBasics::colorBasics(const QString &str)
{
    method = str;
}

colorBasics::colorBasics(const colorBasics &colBas)
{
    method = colBas.get_method();
}

colorBasics::~colorBasics()
{

}

void colorBasics::process(cv::Mat &inMat, cv::Mat &outMat)
{
    if( method.compare("invert") == 0 )
    {
        invert( inMat, outMat );
    }

    if( method.compare("equalize") == 0 )
    {
        equalize( inMat, outMat );
    }

    if( method.compare("2rgb") == 0 )
    {
        convertToRGB( inMat, outMat );
    }

    if( method.compare("2hsv") == 0 )
    {
        convertToHSV( inMat, outMat );
    }

    if( method.compare("2gray") == 0 )
    {
        convertToGRAY( inMat, outMat );
    }
}

void colorBasics::invert(cv::Mat &inMat, cv::Mat &outMat)
{
    outMat = cv::Scalar::all(255) - inMat;
}

void colorBasics::equalize(cv::Mat &inMat, cv::Mat &outMat)
{
    vector<cv::Mat> bgr_planes;
    cv::split(inMat, bgr_planes);
    cv::equalizeHist(bgr_planes[0],bgr_planes[0]);
    cv::equalizeHist(bgr_planes[1],bgr_planes[1]);
    cv::equalizeHist(bgr_planes[2],bgr_planes[2]);
    cv::merge(bgr_planes,outMat);
}

void colorBasics::convertToRGB(cv::Mat &inMat, cv::Mat &outMat)
{
    cv::cvtColor(inMat, outMat, cv::COLOR_BGR2RGB);
}

void colorBasics::convertToHSV(cv::Mat &inMat, cv::Mat &outMat)
{
    cv::cvtColor(inMat, outMat, cv::COLOR_BGR2HSV);
}

void colorBasics::convertToGRAY(cv::Mat &inMat, cv::Mat &outMat)
{
    cv::cvtColor(inMat, outMat, cv::COLOR_BGR2GRAY);
}

void colorBasics::set_method(const QString &str)
{
    method = str;
}
