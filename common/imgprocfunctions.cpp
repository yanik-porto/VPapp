#include "imgprocfunctions.h"

void getHistogram(const cv::Mat &inMat, cv::Mat histImage[3])
{
    /// Separate the image in 3 places ( B, G and R )
    vector<cv::Mat> bgr_planes;
    split(inMat, bgr_planes );

    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    cv::Mat b_hist, g_hist, r_hist;

    cv::calcHist( &bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    cv::calcHist( &bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    cv::calcHist( &bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

    // Draw the histograms for B, G and R
     int hist_w = 512; int hist_h = 400;
     int bin_w = cvRound( (double) hist_w/histSize );

     /// Normalize the result to [ 0, histImage.rows ]
     cv::normalize(b_hist, b_hist, 0, histImage[0].rows, cv::NORM_MINMAX, -1, cv::Mat() );
     cv::normalize(g_hist, g_hist, 0, histImage[1].rows, cv::NORM_MINMAX, -1, cv::Mat() );
     cv::normalize(r_hist, r_hist, 0, histImage[2].rows, cv::NORM_MINMAX, -1, cv::Mat() );

     /// Draw for each channel
     for( int i = 1; i < histSize; i++ )
     {
         cv::line( histImage[0], cv::Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                          cv::Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                          cv::Scalar( 255, 0, 0), 2, 8, 0  );
         cv::line( histImage[1], cv::Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                          cv::Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                          cv::Scalar( 0, 255, 0), 2, 8, 0  );
         cv::line( histImage[2], cv::Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                          cv::Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                          cv::Scalar( 0, 0, 255), 2, 8, 0  );
     }
}

void getProfile(const cv::Mat &inMat, const cv::Point &p1, const cv::Point &p2, cv::Mat histImage[3])
{
    cv::LineIterator lineIt(inMat, p1, p2, 8);
    std::vector<cv::Vec3b> buf(static_cast<size_t>(lineIt.count));
    for(size_t i = 0; i < lineIt.count; i++, ++lineIt) {
        buf[i] = *(const cv::Vec3b*)*lineIt;
    }

    size_t histWidth = static_cast<size_t>(histImage[0].cols);
    size_t histHeight = static_cast<size_t>(histImage[0].rows);
    int b0 = 0;
    int g0 = 0;
    int r0 = 0;
    for (size_t i = 0; i < histWidth; i++) {
        size_t bufIdx = static_cast<size_t>(i / static_cast<float>(histWidth) * buf.size());
        int b1 = static_cast<int>(buf[bufIdx][0] / 255.f * histHeight);
        int g1 = static_cast<int>(buf[bufIdx][1] / 255.f * histHeight);
        int r1 = static_cast<int>(buf[bufIdx][2] / 255.f * histHeight);
        if (i > 0) {
            cv::line(histImage[0], cv::Point(i-1, b0), cv::Point(i, b1), cv::Scalar(255, 0, 0), 2);
            cv::line(histImage[1], cv::Point(i-1, g0), cv::Point(i, g1), cv::Scalar(0, 255, 0), 2);
            cv::line(histImage[2], cv::Point(i-1, r0), cv::Point(i, r1), cv::Scalar(0, 0, 255), 2);
        }
        b0 = b1;
        g0 = g1;
        r0 = r1;
    }

}

float distancePointLine(const cv::Point_<double> point, const cv::Vec<double,3>& line)
{
    //Line is given as a*x + b*y + c = 0
    std::fabs(line(0)*point.x + line(1)*point.y + line(2))
          / std::sqrt(line(0)*line(0)+line(1)*line(1));
}
