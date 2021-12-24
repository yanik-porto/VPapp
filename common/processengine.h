#ifndef PROCESSENGINE_H
#define PROCESSENGINE_H

#include "imgprocfunctions.h"
#include "methods/ipmethod.h"
#include "methods/sandp.h"
#include "methods/filters.h"
#include "methods/derivatives.h"
#include "methods/featuresPts.h"
#include "methods/colorbasics.h"
#include "methods/flipimg.h"
#include "methods/resizeimg.h"
#include "methods/houghtrans.h"
#include "methods/addlogo.h"
#include "methods/channelsfilter.h"

#include <QObject>
#include <QString>
#include <QFileDialog>
#include <iostream>
#include <stdarg.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>


using namespace std;
//using namespace cv::xfeatures2d;

class processEngine : public QObject
{
    Q_OBJECT

private:
    /**
     * original image and processed image
     */
    cv::Mat inI;
    cv::Mat outI;

    /**
     * list of methods
     */
    vector<ipmethod*> ipmethodList;

    /**
     * method containing features (will tell if some are already computed or not)
     */
    FeaturesPts *featPts;

    /**
     * method containing channels filter
     */
    ChannelsFilter *channelsFilter;

public:

    /**
     * constructors and destructors
     */
    explicit processEngine(QObject *parent = 0);
    ~processEngine();

    /**
     * enter image inside the class
     */
    void loadImg(const cv::Mat &matImg);

    /**
     * main function where all the process are applied
     */
    void addProcess(const QString &str, va_list args);


    /**
     * compute the histogram of every channel and store the in the input matrices
     */
    void computeHist(cv::Mat histImage[3]);

    /**
     * Accessors
     */
    const cv::Mat &get_processedImg() const {return outI;}

    const cv::Mat &get_originalImg() const {return inI;}

    /**
     * function which executes all methods in the list
     */
    void process();

    /**
     * reset the list of processes
     */
    void reset();

    /**
     * remove one method of the list giving its index
     */
    void removeMethod(const int &ind);

    /**
     * return the keypoints if they have been computed previously
     */
    void getKeypoints(vector<cv::KeyPoint> &, QString &);

    /**
     * @brief get the list of process
     * @return The list of process as const
     */
    const std::vector<ipmethod*> getProcessList() const;

signals:
    /**
     * signals telling an image has been input or processed and is ready
     */
    void ImgReadyOut();
    void ImgReadyIn();

public slots:
    /**
     * convert the image to BGR
     * called everytime an image is ready to output so that we can apply any kind of method on it after
     */
    void changeToBGR();

};

#endif // PROCESSENGINE_H
