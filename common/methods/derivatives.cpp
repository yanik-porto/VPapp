#include "derivatives.h"

Derivatives::Derivatives()
{

}

Derivatives::Derivatives(const QString &str)
{
    method = str;
}

Derivatives::Derivatives(const Derivatives &der)
{
    set_method(der.method);
}

Derivatives::~Derivatives()
{

}

void Derivatives::process(cv::Mat &inMat, cv::Mat &outMat)
{
    if( method.compare("sobel") == 0 )
    {
        sobel( inMat, outMat );
    }

    if( method.compare("laplacian") == 0 )
    {
        laplacian( inMat, outMat );
    }

    if( method.compare("canny") == 0 )
    {
        canny( inMat, outMat );
    }
}

void Derivatives::set_method(const QString &str)
{
    method = str;
}

void Derivatives::sobel(cv::Mat &inMat, cv::Mat &outMat)
{
    /// Convert image to gray
    cv::cvtColor( inMat, outMat, cv::COLOR_BGR2GRAY );

    /// Generate gradient images and set parameter of the depth to 16S
    cv::Mat grad_x, grad_y;
    int ddepth = CV_16S;

    ///Apply sobel
    cv::Sobel( outMat, grad_x,  ddepth, 1, 0 );
    cv::Sobel( outMat, grad_y,  ddepth, 0, 1 );

    ///convert our partial results back to CV_8U
    cv::convertScaleAbs( grad_x, grad_x );
    cv::convertScaleAbs( grad_y, grad_y );

    ///Combine both gradients. Total Gradient (approximate)
    cv::addWeighted(grad_x, 0.5, grad_y, 0.5, 0, outMat);
}

void Derivatives::laplacian(cv::Mat & inMat, cv::Mat &outMat)
{
    /// Convert image to gray
    cvtColor( inMat, outMat, cv::COLOR_BGR2GRAY );

    ///Set parameter of the depth to 16S
    int ddepth = CV_16S;

    ///Generate intermediate image and apply Laplacian
    cv::Mat lapl16;
    cv::Laplacian(outMat, lapl16, ddepth,3);

    ///convert to CV_8U
    cv::convertScaleAbs(lapl16, outMat);
}

void Derivatives::canny(cv::Mat & inMat, cv::Mat &outMat)
{
    /// Convert image to gray
    cvtColor( inMat, outMat, cv::COLOR_BGR2GRAY );

    /// Set the parameters
    int lowThresh = 100;
    int ratio = 3;

    /// Apply Canny
    cv::Mat detectedEdges;
    cv::Canny( outMat, outMat, lowThresh, lowThresh*ratio );
}

QString Derivatives::toStr()
{
    return "derivatives";
}

