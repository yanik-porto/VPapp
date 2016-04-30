#ifndef IMGPROCFUNCTIONS_H
#define IMGPROCFUNCTIONS_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <QString>

using namespace std;

void sp_noise(const cv::Mat &inMat, cv::Mat &outMat);
void addLogo(const QString &fileLogo, const cv::Mat &inMat, cv::Mat &outMat);
void getHistogram(const cv::Mat &inMat, cv::Mat histImage[3]);

//template <typename T>
float distancePointLine(const cv::Point_<double> point, const cv::Vec<double,3>& line);

#endif // IMGPROCFUNCTIONS_H
