#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * Constructor and Destructor
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    inFrame("Input"),
    outFrame("Output"),
    cap(0),
    delay(10)
{
    ui->setupUi(this);
    screen = QApplication::desktop()->screenGeometry();cout<<screen.height()<<endl;
    this->setGeometry(0,0,screen.width()/2-68,screen.height());

    procEng = new processEngine;
    QObject::connect(this,SIGNAL(qInputImageReady()),this,SLOT(Display_inImg()));
    QObject::connect(procEng, SIGNAL(ImgReadyOut()), this, SLOT(Display_outImg()));
    QObject::connect(procEng, SIGNAL(ImgReadyOut()), this, SLOT(Disable_widgets()));

    cv::moveWindow(outFrame,screen.width()/2,0);
}

MainWindow::~MainWindow()
{
    delete procEng;
    delete ui;
}

/**
 * Function for displaying images
 */

int MainWindow::show_cv(const String &winname,const Mat &image, const int &delay)
{
    cv::namedWindow(winname);
    cv::imshow(winname, image);
    int ch;
    ch = cv::waitKey(delay);
    return ch;
}

/**
 * Slots called by processengine class signals
 */

void MainWindow::Display_inImg()
{
    ui->label_inputI->setPixmap(QPixmap::fromImage(originalImg.scaled(ui->label_inputI->size(), Qt::KeepAspectRatio, Qt::FastTransformation)));
}

void MainWindow::Display_outImg()
{
    show_cv(outFrame,procEng->get_processedImg(),delay);
}

void MainWindow::Disable_widgets()
{
    int type_img = procEng->get_processedImg().type();

    switch(type_img)
    {
    case 0 :
        ui->tabWidget->widget(2)->setEnabled(false);
        ui->pushButton_hist->setEnabled(false);
        ui->pushButton_eq->setEnabled(false);
        break;
    case 16 :
        ui->tabWidget->widget(2)->setEnabled(true);
        ui->pushButton_hist->setEnabled(true);
        ui->pushButton_eq->setEnabled(true);
        break;
    default: break;
    }

}


/**
 * Slots for buttons opening frames or live stream (main actions)
 */

void MainWindow::on_pushButton_open_clicked()
{
    //Get path to image and read image into cv matrix
    QString filename = QFileDialog::getOpenFileName();
    if(filename.compare("")!=0)
    {
        I = imread(filename.toStdString(),CV_LOAD_IMAGE_COLOR);
        procEng->loadImg(I);

        //Display the input image in the GUI
        originalImg = QImage(filename);
        qInputImageReady();
    }
}

void MainWindow::on_pushButton_webcam_clicked()
{
    Mat frame,flipped,resized,img,blurred,edges;
    int ch,delay=10;

    while(cap.read(frame))
    {
//        ch = show_cv("Input", frame, delay);
//        Canny(frame, edges, 50, 150);
//        ch = show_cv("Canny",edges,delay);
        procEng->loadImg(frame);
        cv::cvtColor( frame, frame, CV_BGR2RGB );
        originalImg=QImage((const unsigned char*)(frame.data), frame.cols,  frame.rows, QImage::Format_RGB888);
        qInputImageReady();
        ch = cv::waitKey(delay);
        if (ch == 27) break;
    }
}

void MainWindow::on_pushButton_stopcam_clicked()
{
    if(cap.isOpened())
    {
        cap.release();
    }
}

void MainWindow::on_pushButton_reset_clicked()
{
    //procEng->addProcess("Reset");
    procEng->reset();
}


/**
 * Slots called by buttons applying a process on the image
 */

void MainWindow::on_pushButton_flip_clicked()
{
    int flipCode = 10;
    bool horiz = ui->checkBox_horiz->isChecked();
    bool vert = ui->checkBox_vert->isChecked();

    if(horiz&&vert)
        flipCode = -1;
    else if(horiz&&!vert)
        flipCode = 1;
    else if(!horiz&&vert)
        flipCode = 0;

    if(flipCode!=10)
        procEng->addProcess("Flip",flipCode);
}

void MainWindow::on_pushButton_Blur_clicked()
{
    int sizeKernel = ui->lineEdit_sizeKernel->text().toInt();
    procEng->addProcess( "blur", sizeKernel );
}

void MainWindow::on_pushButton_kernel_clicked()
{
    int cntKernel = ui->lineEdit_kernelCnt->text().toInt();
    procEng->addProcess( "sharpen", cntKernel );
}

void MainWindow::on_pushButton_morph_clicked()
{
    QModelIndex index = ui->listWidget->currentIndex();
    int sizeElmt = ui->lineEdit_sizeElmt->text().toInt();

    switch(index.row())
    {
    case 0: procEng->addProcess( "erode", sizeElmt );
        break;
    case 1: procEng->addProcess( "dilate", sizeElmt );
        break;
    case 2: procEng->addProcess( "open", sizeElmt );
        break;
    case 3: procEng->addProcess( "close", sizeElmt );
        break;
    default: break;
    }
}

void MainWindow::on_pushButton_sp_clicked()
{
    double rate = double(ui->horizontalSlider_sandp->value())/100;
    procEng->addProcess("SP", rate);
}

void MainWindow::on_pushButton_logo_clicked()
{
    procEng->addProcess("Logo");

}

void MainWindow::on_pushButton_invert_clicked()
{
    procEng->addProcess("Invert");
}

void MainWindow::on_pushButton_hist_clicked()
{
    ///Instantiate the matrices
    cv::Mat histImage[3];

    ///Get the histograms from processEngine class
    procEng->computeHist(histImage);

    for(int i=0; i<3; i++)
    {
        cv::cvtColor(histImage[i], histImage[i], CV_BGR2RGB);
    }

    QImage histR((const unsigned char*)(histImage[2].data), histImage[2].cols,  histImage[2].rows, QImage::Format_RGB888);
    ui->label_histR->setPixmap(QPixmap::fromImage(histR.scaled(ui->label_histR->size(), Qt::KeepAspectRatio, Qt::FastTransformation)));

    QImage histG((const unsigned char*)(histImage[1].data), histImage[1].cols,  histImage[1].rows, QImage::Format_RGB888);
    ui->label_histG->setPixmap(QPixmap::fromImage(histG.scaled(ui->label_histG->size(), Qt::KeepAspectRatio, Qt::FastTransformation)));

    QImage histB((const unsigned char*)(histImage[0].data), histImage[0].cols,  histImage[0].rows, QImage::Format_RGB888);
    ui->label_histB->setPixmap(QPixmap::fromImage(histB.scaled(ui->label_histB->size(), Qt::KeepAspectRatio, Qt::FastTransformation)));


//    /// Display each color histogram
//     cv::namedWindow("calcHist Demo1", CV_WINDOW_KEEPRATIO );
//     cv::imshow("calcHist Demo1", histImage[0] );
//     cv::waitKey(10);

//     cv::namedWindow("calcHist Demo2", CV_WINDOW_KEEPRATIO );
//     cv::imshow("calcHist Demo2", histImage[1] );
//     cv::waitKey(10);

//     cv::namedWindow("calcHist Demo3", CV_WINDOW_KEEPRATIO );
//     cv::imshow("calcHist Demo3", histImage[2] );
//     cv::waitKey(10);
}

void MainWindow::on_pushButton_eq_clicked()
{
    procEng->addProcess("Equalize");
}

void MainWindow::on_pushButton_resize_clicked()
{
    int height = ui->lineEdit_height->text().toInt();
    int width = ui->lineEdit_width->text().toInt();
    procEng->addProcess( "Resize", height, width );
}

void MainWindow::on_pushButton_sobel_clicked()
{
    procEng->addProcess("sobel");
}

void MainWindow::on_pushButton_lapl_clicked()
{
    procEng->addProcess("laplacian");
}

void MainWindow::on_pushButton_canny_clicked()
{
    procEng->addProcess("canny");
}

void MainWindow::on_pushButton_circles_clicked()
{
    int thresh = ui->horizontalSlider_circleThresh->value();
    procEng->addProcess( "Circles", thresh );
}

void MainWindow::on_pushButton_lines_clicked()
{
    int thresh = ui->horizontalSlider_lineThresh->value();
    procEng->addProcess( "Lines", thresh );
}

void MainWindow::on_pushButton_harris_clicked()
{
    int thresh = ui->horizontalSlider_harrisThresh->value();
    procEng->addProcess( "Harris", thresh );
}

void MainWindow::on_pushButton_surf_clicked()
{
    int thresh = ui->horizontalSlider_SURFThresh->value();
    procEng->addProcess( "SURF", thresh );
}

void MainWindow::on_pushButton_sift_clicked()
{
    int nPts = ui->horizontalSlider_SIFTThresh->value();
    procEng->addProcess( "SIFT", nPts );
}

void MainWindow::on_pushButton_fast_clicked()
{
    int thresh = ui->horizontalSlider_FASTThresh->value();
    procEng->addProcess( "FAST", thresh );
}


/**
 * Slots for actions in gui
 */
void MainWindow::on_horizontalSlider_harrisThresh_sliderMoved(int position)
{
    ui->label_threshvalue->setText(QString::number(position));
}

void MainWindow::on_horizontalSlider_SURFThresh_sliderMoved(int position)
{
    ui->label_threshvalue_2->setText(QString::number(position));
}

void MainWindow::on_horizontalSlider_SIFTThresh_sliderMoved(int position)
{
    ui->label_threshvalue_3->setText(QString::number(position));
}

void MainWindow::on_horizontalSlider_FASTThresh_sliderMoved(int position)
{
    ui->label_threshvalue_4->setText(QString::number(position));
}
