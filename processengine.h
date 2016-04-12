#ifndef PROCESSENGINE_H
#define PROCESSENGINE_H

#include "imgprocfunctions.h"

#include <QObject>
#include <QString>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

using namespace std;

class processEngine : public QObject
{
    Q_OBJECT
public:
    explicit processEngine(QObject *parent = 0);
    ~processEngine();

    void loadImg(const QString &filename);
    void addProcess(const QString &str);
    void displayHist();
    const cv::Mat &get_processedImg() const {return outI;}
    const cv::Mat &get_originalImg() const {return inI;}

private:
    cv::Mat inI;
    cv::Mat outI;

signals:
    void ImgReadyOut();
    void ImgReadyIn();

public slots:
    void changeToGray();

};

#endif // PROCESSENGINE_H
