#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "processengine.h"
#include "imgprocfunctions.h"

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

private:
    /**
     * Instance of the gui
     */
    Ui::MainWindow *ui;

    /**
     * status which will tell if a stream is being read, an image is opened or not
     */
    QString mode;

    /**
     * dimensions of the user monitor
     */
    QRect screen;

    /**
     * instances of the processEngine class for the two frames
     */
    processEngine *procEng;
    processEngine *procEng2;

    /**
     * frame being read
     */
    Mat I;

    /**
     * name of the different frames
     */
    string outFrame;
    string outFrame2;
    string inFrame;

    /**
     * Video readers and writer
     */
    cv::VideoCapture cap;
    cv::VideoCapture cap2;
    cv::VideoWriter vidWriter;

    /**
     * delay for the waitkey function
     */
    int delay;

    /**
     * input image with a type readable in the gui
     */
    QImage originalImg;
    QImage originalImg2;

    /**
     * selected frame
     */
    short selectImg;

    /**
     * list of matched keypoints
     */
    vector<cv::DMatch> matches;

    /**
     * tell if enough checkerboard have been detected for the calibration
     */
    bool calibrationEnabled;

    /**
     * number of checkerboard detected
     */
    int nCorners;

    /**
     * list of points for the calibration
     */
    vector< vector<Point3f> > object_points;
    vector< vector<Point2f> > image_points;

    /**
     * fundamental and homography matrices
     */
    cv::Mat F, H;

    /**
     * static attributes for mouse events
     */
    static cv::Point p1;
    static cv::Point p2;
    static bool lineOver;
    static bool lineDrawing;

    /**
     * function for opening a window, calling show and waitkey at the same time
     */
    int show_cv(const String &winname, const Mat &image, const int &delay);

    void dispHistImages(cv::Mat histImage[3]);

    void fillHistogram(const cv::Rect& roi = cv::Rect());

    void fillProfile(const cv::Point &p1, const cv::Point &p2);

    static void onMouseCvImg(int event, int x, int y, int f, void *);
public:

    /**
     * constructor and mutators
     */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /**
     * request a process to the instance of the class processEngine
     */
    void sendProcessRequest(const int &, const QString &, ...);

    /**
     * write the name of a new process inside the list of the gui
     */
    void updateListProcess(const QString &);

    /**
     * update the name of all process inside the list of the gui
     */
    void updateListProcess();

    /**
     * Find the corners of the checkerboard patter for the calibration
     */
    bool findCorners(vector< vector<Point3f> > &object_points, vector< vector<Point2f> > &image_points, Mat image);

    /**
     * Calibrate and undistort the images captured by the webcam
     */
    cv::Mat calibrateAndUndistort(const vector< vector<Point3f> > &object_points, const vector< vector<Point2f> > &image_points, const Mat &image);


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

    void on_pushButton_2rgb_clicked();

    void on_pushButton_2hsv_clicked();

    void on_pushButton_2gray_clicked();

    void on_pushButton_warp_clicked();

    void on_pushButton_homo_clicked();

    void on_pushButton_contours_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_stopRec_clicked();

    void on_pushButton_updChannels_clicked();

    void on_horizontalSlider_c1_sliderMoved(int position);

    void on_horizontalSlider_c2_sliderMoved(int position);

    void on_horizontalSlider_c3_sliderMoved(int position);

    void on_horizontalSlider_c1_max_sliderMoved(int position);

    void on_horizontalSlider_c2_max_sliderMoved(int position);

    void on_horizontalSlider_c3_max_sliderMoved(int position);

    void on_pushButton_area_clicked();

    void on_pushButton_profile_clicked();

public slots:


};

#endif // MAINWINDOW_H
