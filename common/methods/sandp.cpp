#include "sandp.h"

SandP::SandP()
{
    rate = 0.5;
}

SandP::SandP( const double &n )
{
    rate = n;
}

SandP::SandP( const SandP & sp)
{
    set_rate( sp.rate );
}

SandP::~SandP()
{

}


void SandP::set_rate(const double &n)
{
    rate = n;
}

void SandP::salt(cv::Mat &inMat, cv::Mat &outMat, int n)
{
    int i,j;

    for ( int k=0; k<n; k++ )
    {
        i = std::rand()%inMat.rows;
        j = std::rand()%inMat.cols;

        if( inMat.type() == CV_8UC1) //gray-scale image
        {
            outMat.at<uchar>( i, j ) = 255;
        }

        else if( inMat.type() == CV_8UC3) //BGR image
        {
            outMat.at<cv::Vec3b>( i, j )[0] = 255;
            outMat.at<cv::Vec3b>( i, j )[1] = 255;
            outMat.at<cv::Vec3b>( i, j )[2] = 255;
        }
    }
}

void SandP::pepper(cv::Mat &inMat, cv::Mat &outMat, int n)
{
    int i,j;

    for ( int k=0; k<n; k++ )
    {
        i = std::rand()%inMat.rows;
        j = std::rand()%inMat.cols;

        if( inMat.type() == CV_8UC1) //gray-scale image
        {
            outMat.at<uchar>( i, j ) = 0;
        }

        else if( inMat.type() == CV_8UC3) //BGR image
        {
            outMat.at<cv::Vec3b>( i, j )[0] = 0;
            outMat.at<cv::Vec3b>( i, j )[1] = 0;
            outMat.at<cv::Vec3b>( i, j )[2] = 0;
        }
    }
}

void SandP::process(cv::Mat &inMat, cv::Mat &outMat)
{
    int nPixels = rate*inMat.cols*inMat.rows; //number of pixels containing noise computed from the rate

    salt( inMat, outMat, nPixels/2 );
    pepper( inMat, outMat, nPixels/2 );

}

QString SandP::toStr()
{
    return "salt and pepper";
}


