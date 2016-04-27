#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "processengine.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QDesktopWidget>

#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>


#include <stdarg.h>
#include <iostream>
#include <vector>
//#include "libusb-master/libusb/libusb.h"

using namespace std;
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * constructor and mutators
     */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void sendProcessRequest(const int &, const QString &, ...);

    void updateListProcess(const QString &);

    bool findCorners(vector< vector<Point3f> > &object_points, vector< vector<Point2f> > &image_points, Mat image);

    cv::Mat calibrateAndUndistort(const vector< vector<Point3f> > &object_points, const vector< vector<Point2f> > &image_points, const Mat &image);

private:
    Ui::MainWindow *ui;

    QRect screen;

    int show_cv(const String &winname, const Mat &image, const int &delay);

    processEngine *procEng;
    processEngine *procEng2;

    Mat I;
    string outFrame;
    string outFrame2;
    string inFrame;

    cv::VideoCapture cap;
    cv::VideoCapture cap2;

    int delay;

    QImage originalImg;
    QImage originalImg2;

    short selectImg;

    vector<cv::DMatch> matches;

    bool calibrationEnabled;

    vector< vector<Point3f> > object_points;
    vector< vector<Point2f> > image_points;
    int nCorners;

signals:
    void qInputImageReady();

private slots:
    /**
     * Slot called from signal of processengine class
     */

    void Display_inImg();

    void Display_outImg();

    void Disable_widgets();

    /**
     * Slots for widget actions
     */

    void on_pushButton_open_clicked();

    void on_pushButton_webcam_clicked();

    void on_pushButton_Blur_clicked();

    void on_pushButton_stopcam_clicked();

    void on_pushButton_flip_clicked();

    void on_pushButton_sp_clicked();

    void on_pushButton_logo_clicked();

    void on_pushButton_invert_clicked();

    void on_pushButton_hist_clicked();

    void on_pushButton_eq_clicked();

    void on_pushButton_resize_clicked();

    void on_pushButton_reset_clicked();

    void on_pushButton_sobel_clicked();

    void on_pushButton_lapl_clicked();

    void on_pushButton_kernel_clicked();

    void on_pushButton_canny_clicked();

    void on_pushButton_circles_clicked();

    void on_pushButton_lines_clicked();

    void on_pushButton_harris_clicked();

    void on_pushButton_surf_clicked();

    void on_pushButton_sift_clicked();

    void on_pushButton_fast_clicked();

    void on_pushButton_morph_clicked();

    void on_horizontalSlider_harrisThresh_sliderMoved(int position);

    void on_horizontalSlider_SURFThresh_sliderMoved(int position);

    void on_horizontalSlider_SIFTThresh_sliderMoved(int position);

    void on_horizontalSlider_FASTThresh_sliderMoved(int position);

    void on_radioButton_img1_clicked(bool checked);

    void on_radioButton_img2_clicked(bool checked);

    void on_pushButton_rmv_clicked();

    void on_pushButton_stitch_clicked();

    void on_pushButton_match_clicked();

    void on_pushButton_FundMat_clicked();

    void on_pushButton_epipol_clicked();

    void on_pushButton_calib_clicked();

    void on_pushButton_video_clicked();

public slots:


};

#endif // MAINWINDOW_H
