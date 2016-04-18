#include "featuresPts.h"

FeaturesPts::FeaturesPts()
{

}

FeaturesPts::FeaturesPts(const QString &str, int &n)
{
    method = str;
    thresh = n;
}

FeaturesPts::FeaturesPts(const FeaturesPts &feat)
{
    method = feat.get_method();
    thresh = feat.get_thresh();
}

FeaturesPts::~FeaturesPts()
{

}

void FeaturesPts::process(cv::Mat &inMat, cv::Mat &outMat)
{
    if( method.compare("Harris") == 0 )
    {
        Harris_keypts( inMat, outMat, thresh );
    }

    if( method.compare("SURF") == 0 )
    {
        SURF_keypts( inMat, outMat, thresh );
    }

    if( method.compare("SIFT") == 0 )
    {
        SIFT_keypts( inMat, outMat, thresh );
    }

    if( method.compare("FAST") == 0 )
    {
        FAST_keypts( inMat, outMat, thresh );
    }
}

void FeaturesPts::Harris_keypts(cv::Mat &inMat, cv::Mat &outMat, const int &n)
{
        ///Convert image to gray
        cv::Mat outMat_gray, corners, corners_scaled;
        cvtColor(inMat, outMat_gray, CV_BGR2GRAY);

        ///Detector Parameters
        int blockSize = 2;
        int apertureSize = 3;
        double k = 0.04;

        ///Detect corners
        cv::cornerHarris( outMat_gray, corners, blockSize, apertureSize, k, cv::BORDER_DEFAULT );

        /// Normalizing
        cv::normalize( corners, corners, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );
        cv::convertScaleAbs( corners, corners_scaled );

        /// Drawing a circle around corners
        for( int j = 0; j < corners.rows ; j++ )
           { for( int i = 0; i < corners.cols; i++ )
                {
                  if( (int) corners.at<float>(j,i) > n )
                    {
                     cv::circle( outMat, cv::Point( i, j ), 5,  cv::Scalar(0,0,255), 2, 8, 0 );
                    }
                }
           }
}

void FeaturesPts::SURF_keypts(cv::Mat &inMat, cv::Mat &outMat, const int &n)
{
    cv::Ptr<SURF> detector = SURF::create( n );

    vector<cv::KeyPoint> keypoints;
    detector->detect( inMat, keypoints );

    cv::drawKeypoints(inMat, keypoints, outMat, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT );
}

void FeaturesPts::SIFT_keypts(cv::Mat &inMat, cv::Mat &outMat, const int &n)
{
    cv::Ptr<cv::Feature2D> f2d = SIFT::create( n );

    vector<cv::KeyPoint> keypoints;
    f2d->detect( inMat, keypoints );

    cv::drawKeypoints( inMat, keypoints, outMat, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT );
}

void FeaturesPts::FAST_keypts(cv::Mat &inMat, cv::Mat &outMat, const int &n)
{
    cv::Mat outMat_gray;
    cv::cvtColor( inMat, outMat_gray, CV_BGR2GRAY);

    vector<cv::KeyPoint> keypoints;
    cv::FAST(outMat_gray, keypoints, n, true);

    cv::drawKeypoints( inMat, keypoints, outMat, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT );
}

void FeaturesPts::set_method(const QString &str)
{
   method = str;
}

void FeaturesPts::set_thresh(const int &n)
{
    thresh = n;
}

