#ifndef IMGPROCFUNCTIONS_H
#define IMGPROCFUNCTIONS_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <QString>

using namespace std;

/**
 * Some functions useful for some computations
 */

//Return the rgb histogram for a given image
void getHistogram(const cv::Mat &inMat, cv::Mat histImage[3]);

//Return the rgb profile of a given line in the image
void getProfile(const cv::Mat &inMat, const cv::Point &p1, const cv::Point &p2, cv::Mat histImage[3]);

//Compute the distance from a point to a line
float distancePointLine(const cv::Point_<double> point, const cv::Vec<double,3>& line);

#endif // IMGPROCFUNCTIONS_H
