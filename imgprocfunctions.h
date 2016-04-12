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

#endif // IMGPROCFUNCTIONS_H
