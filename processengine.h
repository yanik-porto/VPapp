#ifndef PROCESSENGINE_H
#define PROCESSENGINE_H

#include "imgprocfunctions.h"
#include "ipmethod.h"
#include "sandp.h"
#include "filters.h"
#include "derivatives.h"
#include "featuresPts.h"
//#include "features.h"

#include <QObject>
#include <QString>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

using namespace std;
//using namespace cv::xfeatures2d;

class processEngine : public QObject
{
    Q_OBJECT
public:

    /**
     * constructors and destructors
     */
    explicit processEngine(QObject *parent = 0);
    ~processEngine();

    /**
     * enter image path inside the class
     */
    //void loadImg(const QString &filename);
    void loadImg(const cv::Mat &matImg);

    /**
     * main function where all the process are applied
     */
    void addProcess(const QString &str, ...);


    void computeHist(cv::Mat histImage[3]);

    /**
     * Accessors
     */
    const cv::Mat &get_processedImg() const {return outI;}

    const cv::Mat &get_originalImg() const {return inI;}



    void process();

    void reset();

private:
    cv::Mat inI;
    cv::Mat outI;

    vector<ipmethod*> ipmethodList;

signals:
    void ImgReadyOut();
    void ImgReadyIn();

public slots:
    void changeToBGR();

};

#endif // PROCESSENGINE_H
