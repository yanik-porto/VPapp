#include "houghtrans.h"

HoughTrans::HoughTrans()
{
    shape = "lines";
    thresh = 100;

}

HoughTrans::HoughTrans(const QString &str, const int &n)
{
    shape = str;
    thresh = n;
}

HoughTrans::HoughTrans(const QString &str)
{
    shape = str;
    thresh = 100;
}

HoughTrans::HoughTrans(const HoughTrans &hT)
{
    shape = hT.get_shape();
    thresh = hT.get_thresh();
}

HoughTrans::~HoughTrans()
{

}

void HoughTrans::process(cv::Mat &inMat, cv::Mat &outMat)
{
    if( shape.compare("lines") == 0 )
    {
        findLines( inMat, outMat, thresh );
    }

    if( shape.compare("circles") == 0 )
    {
        findCircles( inMat, outMat, thresh );
    }

    if( shape.compare("contours") == 0 )
    {
        findContour( inMat, outMat );
    }
}

void HoughTrans::findLines(cv::Mat &inMat, cv::Mat &outMat, int &n)
{
    ///Convert image to gray
    cv::Mat outMat_gray;
    cvtColor(inMat, outMat_gray, CV_BGR2GRAY);

    ///Create vector to store lines
    vector<cv::Vec4i> lines;

    ///Apply hough transform
    cv::HoughLinesP( outMat_gray, lines, 1, CV_PI/180, n, 50, 10 );

    ///Draw the detected lines
    for( size_t i = 0; i < lines.size(); i++ )
    {
      cv::Vec4i l = lines[i];
      cv::line( outMat, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 1, CV_AA);
    }
}

void HoughTrans::findCircles(cv::Mat &inMat, cv::Mat &outMat, int &n)
{
    ///Convert image to gray
    cv::Mat outMat_gray;
    cvtColor( inMat, outMat_gray, CV_BGR2GRAY);

    ///Create vectors to store x,y and r
    vector<cv::Vec3f> circles;

    ///Apply the hough transform to find circles
    cv::HoughCircles( outMat_gray, circles, CV_HOUGH_GRADIENT, 1, outMat_gray.rows/8, 200, n, 0, 0 ); //Play with parameter 2

    ///Draw detected circles
    for( size_t i = 0; i < circles.size(); i++ )
    {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        cv::circle( outMat, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
        // circle outline
        cv::circle( outMat, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
     }
}

void HoughTrans::findContour(cv::Mat &inMat, cv::Mat &outMat)
{
    cv::Mat outGray;
    cv::cvtColor(inMat, outGray, CV_BGR2GRAY );
    std::vector< std::vector<cv::Point> >	contours;
    cv::findContours(outGray, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    cv::drawContours(outMat, contours, -1, cv::Scalar(255,0,0), 2);


}

void HoughTrans::set_shape(const QString &str)
{
    shape = str;
}

void HoughTrans::set_thresh(const int &n)
{
    thresh = n;
}
