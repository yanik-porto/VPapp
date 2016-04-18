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



#include <iostream>
#include <vector>

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

private:
    Ui::MainWindow *ui;

    QRect screen;

    int show_cv(const String &winname, const Mat &image, const int &delay);

    processEngine *procEng;

    Mat I;
    string outFrame;
    string inFrame;

    VideoCapture cap;

    int delay;

    QImage originalImg;

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

public slots:


};

#endif // MAINWINDOW_H
