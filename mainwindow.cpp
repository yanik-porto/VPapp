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
    outFrame2("Output2"),
    delay(10),
    selectImg(1),
    calibrationEnabled(false),
    nCorners(0)
{
    ui->setupUi(this);
    screen = QApplication::desktop()->screenGeometry();cout<<screen.height()<<endl;
    this->setGeometry(0,0,screen.width()/2-68,screen.height());

    procEng = new processEngine;
    procEng2 = new processEngine;

    QObject::connect(this,SIGNAL(qInputImageReady()),this,SLOT(Display_inImg()));
    QObject::connect(procEng, SIGNAL(ImgReadyOut()), this, SLOT(Display_outImg()));
//    QObject::connect(procEng, SIGNAL(ImgReadyOut()), this, SLOT(Disable_widgets()));

    QObject::connect(procEng2, SIGNAL(ImgReadyOut()), this, SLOT(Display_outImg()));

    cv::moveWindow(outFrame,screen.width()/2,0);

    ui->listWidget_process->setEnabled(true);
    ui->listWidget_process_2->setEnabled(false);
    ui->label_img1->setEnabled(true);
    ui->label_img2->setEnabled(false);

//    Disable_widgets("");

//    CvCapture* capture = cvCaptureFromCAM( CV_CAP_DSHOW);
//    capture->VI.listDevices();
//    for(int device = 0; device<10; device++)
//    {
//        VideoCapture capTry(device);
//        if (!capTry.isOpened())
//            ui->comboBox_devices->addItem(QString::number(device));

//    }

    for(int i=-1; i<4; i++)
    {
        ui->comboBox_devices->addItem(QString::number(i));
    }
//    libusb_device** usbdevices;

//    int list = libusb_get_device_list(NULL, &usbdevices);
}

MainWindow::~MainWindow()
{
    //Does not work
    if(cap.isOpened())
    {

        cap.release();
    }

    delete procEng;
    delete ui;
}

/**
  * Function for applying a process
  */

void MainWindow::sendProcessRequest(const int &frame, const QString &str, ...)
{
    va_list vl;
    va_start(vl, str);

    switch(frame)
    {
    case 1: procEng->addProcess(str, vl);
        break;
    case 2: procEng2->addProcess(str, vl);
        break;
    default:
        break;
    }

    va_end(vl);

}

void MainWindow::updateListProcess(const QString &str)
{
    switch(selectImg)
    {
    case 1:
        ui->listWidget_process->addItem( str );
        ui->listWidget_process->setCurrentRow( ui->listWidget_process->count() - 1 );
        break;
    case 2:
        ui->listWidget_process_2->addItem( str );
        ui->listWidget_process_2->setCurrentRow( ui->listWidget_process_2->count() - 1 );
        break;
    default:
        break;
    }
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
    switch(selectImg)
    {
    case 1:
        if(!originalImg.isNull())
            ui->label_inputI->setPixmap(QPixmap::fromImage(originalImg.scaled(ui->label_inputI->size(), Qt::KeepAspectRatio, Qt::FastTransformation)));
        else
            ui->label_inputI->setPixmap(NULL);
        break;
    case 2:
        if(!originalImg2.isNull())
            ui->label_inputI->setPixmap(QPixmap::fromImage(originalImg2.scaled(ui->label_inputI->size(), Qt::KeepAspectRatio, Qt::FastTransformation)));
        else
            ui->label_inputI->setPixmap(NULL);
        break;
    default:
        break;
    }
}

void MainWindow::Display_outImg()
{
    switch(selectImg)
    {
    case 1: show_cv(outFrame,procEng->get_processedImg(),delay);
        if(!ui->lineEdit_width->isActiveWindow())
            ui->lineEdit_width->setText(QString::number(procEng->get_processedImg().cols));
        if(!ui->lineEdit_height->isActiveWindow())
            ui->lineEdit_height->setText(QString::number(procEng->get_processedImg().rows));
        break;
    case 2: show_cv(outFrame2, procEng2->get_processedImg(), delay);
        if(!ui->lineEdit_width->isActiveWindow())
            ui->lineEdit_width->setText(QString::number(procEng2->get_processedImg().cols));
        if(!ui->lineEdit_height->isActiveWindow())
            ui->lineEdit_height->setText(QString::number(procEng2->get_processedImg().rows));
        break;
    default:
        break;
    }


}

void MainWindow::Disable_widgets(const QString &mode)
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
    default:
        ui->tabWidget->widget(2)->setEnabled(true);
        ui->pushButton_hist->setEnabled(true);
        ui->pushButton_eq->setEnabled(true);
        break;
    }

    vector<cv::KeyPoint> keypts1, keypts2;
    QString method1, method2;
    procEng->getKeypoints(keypts1,method1);
    procEng->getKeypoints(keypts1,method2);
    if((keypts1.size() !=0 ) && (keypts2.size()) !=0 && (method1.compare(method2)==0))
    {
        ui->pushButton_match->setEnabled(true);
//        ui->pushButton_FundMat->setEnabled();
    }
    else
    {
       ui->pushButton_match->setEnabled(false);
//       ui->pushButton_FundMat->setEnabled(false);
    }



    if(mode.compare("reading")==0)
    {
        ui->pushButton_open->setEnabled(false);
        ui->pushButton_video->setEnabled(false);
        ui->pushButton_webcam->setEnabled(false);
        ui->radioButton_img1->setEnabled(false);
        ui->radioButton_img2->setEnabled(false);
        ui->pushButton_match->setEnabled(false);
        ui->pushButton_stitch->setEnabled(false);
    }

    if(mode.compare("stop")==0)
    {
        ui->pushButton_open->setEnabled(true);
        ui->pushButton_video->setEnabled(true);
        ui->pushButton_webcam->setEnabled(true);
        ui->radioButton_img1->setEnabled(true);
        ui->radioButton_img2->setEnabled(true);
        ui->pushButton_match->setEnabled(true);
        ui->pushButton_stitch->setEnabled(true);
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
        switch(selectImg)
        {
        case 1: procEng->loadImg(I);
            originalImg = QImage(filename);
            break;
        case 2: procEng2->loadImg(I);
            originalImg2 = QImage(filename);
            break;
        default:
            break;
        }

        //Display the input image in the GUI
//        originalImg = QImage(filename);

        emit qInputImageReady();
    }
}

void MainWindow::on_pushButton_webcam_clicked()
{
    cv::Mat frame;
    int ch,delay=10;
    int device = ui->comboBox_devices->currentText().toInt();
    bool detectedCorners = false;

    switch(selectImg)
    {
    case 1:
        cap.open(device);
        while(cap.read(frame))
        {
            if(calibrationEnabled)
            {
                if(nCorners<=5)
                    detectedCorners = findCorners(object_points, image_points, frame);

                if(detectedCorners)
                    nCorners++;

                if(nCorners>5)
                {
                    frame = calibrateAndUndistort(object_points, image_points, frame);
                    cv::destroyWindow("calibration");
                }
            }
            detectedCorners = false;

            procEng->loadImg(frame);
            cv::cvtColor( frame, frame, CV_BGR2RGB );
            originalImg=QImage((const unsigned char*)(frame.data), frame.cols,  frame.rows, QImage::Format_RGB888);
            qInputImageReady();
            ch = cv::waitKey(delay);
            if (ch == 27)
            {
                Disable_widgets("stop");
                break;
            }

            Disable_widgets("reading");

        }
        break;

    case 2:
        cap2.open(device);
        while(cap2.read(frame))
        {
            if(calibrationEnabled)
            {
                if(nCorners<=5)
                    detectedCorners = findCorners(object_points, image_points, frame);

                if(detectedCorners)
                    nCorners++;

                if(nCorners>5)
                {
                    frame = calibrateAndUndistort(object_points, image_points, frame);
                    cv::destroyWindow("calibration");
                }
            }
            detectedCorners = false;

            procEng2->loadImg(frame);
            cv::cvtColor( frame, frame, CV_BGR2RGB );
            originalImg2=QImage((const unsigned char*)(frame.data), frame.cols,  frame.rows, QImage::Format_RGB888);
            qInputImageReady();
            ch = cv::waitKey(delay);
            if (ch == 27)
            {
                Disable_widgets("stop");
                break;
            }

            Disable_widgets("reading");
        }
        break;

    default:
        break;
    }
}

void MainWindow::on_pushButton_stopcam_clicked()
{
    switch(selectImg)
    {
    case 1:
        if(cap.isOpened())
        {
            cap.release();
        }
        break;
    case 2:
        if(cap2.isOpened())
        {
            cap2.release();
        }
        break;
    default:
        break;
    }

    Disable_widgets("stop");
}

void MainWindow::on_pushButton_video_clicked()
{
    //Get path to video and read video into cv matrix
    QString filename = QFileDialog::getOpenFileName();
    cv::Mat frame;
    int ch,delay=10;

    if(filename.compare("")!=0)
    {
        switch(selectImg)
        {
        case 1: cap.open(filename.toStdString());
            while(cap.read(frame))
            {
                procEng->loadImg(frame);
                cv::cvtColor( frame, frame, CV_BGR2RGB );
                originalImg=QImage((const unsigned char*)(frame.data), frame.cols,  frame.rows, QImage::Format_RGB888);
                qInputImageReady();
                if (ch == 27)
                {
                    Disable_widgets("stop");
                    break;
                }

                Disable_widgets("reading");
            }
            break;
        case 2:
            cap2.open(filename.toStdString());
            while(cap2.read(frame))
            {
                procEng2->loadImg(frame);
                cv::cvtColor( frame, frame, CV_BGR2RGB );
                originalImg2=QImage((const unsigned char*)(frame.data), frame.cols,  frame.rows, QImage::Format_RGB888);
                qInputImageReady();
                ch = cv::waitKey(delay);
                if (ch == 27)
                {
                    Disable_widgets("stop");
                    break;
                }

                Disable_widgets("reading");
            }
            break;
        default:
            break;
        }

        //Display the input image in the GUI
//        originalImg = QImage(filename);

        emit qInputImageReady();
    }
}




void MainWindow::on_pushButton_reset_clicked()
{
    //procEng->addProcess("Reset");
    switch(selectImg)
    {
    case 1: procEng->reset();
        ui->listWidget_process->clear();
        break;
    case 2: procEng2->reset();
        ui->listWidget_process_2->clear();
        break;
    default:
        break;
    }



}

void MainWindow::on_pushButton_rmv_clicked()
{
    int row;

    switch(selectImg)
    {
    case 1: row = ui->listWidget_process->currentRow();
        delete ui->listWidget_process->takeItem(row);
        procEng->removeMethod(row);
        break;
    case 2: row = ui->listWidget_process_2->currentRow();
        delete ui->listWidget_process_2->takeItem(row);
        procEng2->removeMethod(row);
        break;
    default:
        break;
    }
}

/**
 * Function for displaying the histograms
 */
void MainWindow::on_pushButton_hist_clicked()
{
    ///Instantiate the matrices
    cv::Mat histImage[3];

    ///Get the histograms from processEngine class
    switch(selectImg)
    {
    case 1:procEng->computeHist(histImage);
        break;
    case 2:procEng2->computeHist(histImage);
        break;
    default:
        break;
    }

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
    {
        sendProcessRequest(selectImg, "flip", flipCode);
        updateListProcess("flip " + QString::number(flipCode));
    }
}

void MainWindow::on_pushButton_Blur_clicked()
{
    int sizeKernel = ui->lineEdit_sizeKernel->text().toInt();
    sendProcessRequest(selectImg, "blur", sizeKernel);
    updateListProcess("blur " + QString::number(sizeKernel));
}

void MainWindow::on_pushButton_kernel_clicked()
{
    int cntKernel = ui->lineEdit_kernelCnt->text().toInt();
    sendProcessRequest( selectImg, "sharpen", cntKernel );
    updateListProcess("sharpen " + QString::number(cntKernel));
}

void MainWindow::on_pushButton_morph_clicked()
{
    QModelIndex index = ui->listWidget->currentIndex();
    int sizeElmt = ui->lineEdit_sizeElmt->text().toInt();

    switch(index.row())
    {
    case 0:
        sendProcessRequest( selectImg, "erode", sizeElmt );
        updateListProcess("erode " + QString::number(sizeElmt));
        break;
    case 1:
        sendProcessRequest( selectImg, "dilate", sizeElmt );
        updateListProcess("dilate " + QString::number(sizeElmt));
        break;
    case 2:
        sendProcessRequest( selectImg, "open", sizeElmt );
        updateListProcess("open " + QString::number(sizeElmt));
        break;
    case 3:
        sendProcessRequest( selectImg, "close", sizeElmt );
        updateListProcess("close " + QString::number(sizeElmt));
        break;
    default: break;
    }
}

void MainWindow::on_pushButton_sp_clicked()
{
    double rate = double(ui->horizontalSlider_sandp->value())/100;
    sendProcessRequest( selectImg, "SP", rate );
    updateListProcess("salt & pepper " + QString::number(rate));
}

void MainWindow::on_pushButton_logo_clicked()
{
    sendProcessRequest( selectImg, "logo" );
    updateListProcess("logo ");
}

void MainWindow::on_pushButton_invert_clicked()
{
    sendProcessRequest( selectImg, "invert" );
    updateListProcess("invert" );
}

void MainWindow::on_pushButton_2rgb_clicked()
{
    sendProcessRequest( selectImg, "2rgb" );
    updateListProcess("to rgb");
}

void MainWindow::on_pushButton_2hsv_clicked()
{
    sendProcessRequest( selectImg, "2hsv" );
    updateListProcess("to hsv");
}

void MainWindow::on_pushButton_2gray_clicked()
{
    sendProcessRequest( selectImg, "2gray" );
    updateListProcess("to gray");
}

void MainWindow::on_pushButton_eq_clicked()
{
    sendProcessRequest( selectImg, "equalize" );
    updateListProcess("equalize" );
}

void MainWindow::on_pushButton_resize_clicked()
{
    int height = ui->lineEdit_height->text().toInt();
    int width = ui->lineEdit_width->text().toInt();
    sendProcessRequest( selectImg, "Resize", height, width );
    updateListProcess("resize " + QString::number(height) + ", " + QString::number(width));
}

void MainWindow::on_pushButton_sobel_clicked()
{
    sendProcessRequest( selectImg, "sobel" );
    updateListProcess("sobel" );
}

void MainWindow::on_pushButton_lapl_clicked()
{
    sendProcessRequest( selectImg, "laplacian" );
    updateListProcess("laplacian" );
}

void MainWindow::on_pushButton_canny_clicked()
{
    sendProcessRequest( selectImg, "canny" );
    updateListProcess("canny" );
}

void MainWindow::on_pushButton_circles_clicked()
{
    int thresh = ui->horizontalSlider_circleThresh->value();
    sendProcessRequest( selectImg, "circles", thresh );
    updateListProcess("hough circles " + QString::number(thresh) );
}

void MainWindow::on_pushButton_lines_clicked()
{
    int thresh = ui->horizontalSlider_lineThresh->value();
    sendProcessRequest( selectImg, "lines", thresh );
    updateListProcess("hough lines " + QString::number(thresh) );
}

void MainWindow::on_pushButton_harris_clicked()
{
    int thresh = ui->horizontalSlider_harrisThresh->value();
    sendProcessRequest( selectImg, "Harris", thresh );
    updateListProcess("harris corners " + QString::number(thresh) );
}

void MainWindow::on_pushButton_surf_clicked()
{
    int thresh = ui->horizontalSlider_SURFThresh->value();
    sendProcessRequest( selectImg, "SURF", thresh );
    updateListProcess("SURF detector " + QString::number(thresh) );

    Disable_widgets("");
}

void MainWindow::on_pushButton_sift_clicked()
{
    int nPts = ui->horizontalSlider_SIFTThresh->value();
    sendProcessRequest( selectImg, "SIFT", nPts );
    updateListProcess("SIFT detector " + QString::number(nPts) );

    Disable_widgets("");
}

void MainWindow::on_pushButton_fast_clicked()
{
    int thresh = ui->horizontalSlider_FASTThresh->value();
    sendProcessRequest( selectImg, "FAST", thresh );
    updateListProcess("FAST detector " + QString::number(thresh) );
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


void MainWindow::on_radioButton_img1_clicked(bool checked)
{
    if(checked == true)
    {
        if(selectImg == 2)
        {

            if(cap.isOpened())
                cap.release();

            selectImg = 1;

            emit qInputImageReady();
            cout<<"image 1 selected"<<endl;
            ui->label_img1->setEnabled(true);
            ui->label_img2->setEnabled(false);
            ui->listWidget_process->setEnabled(true);
            ui->listWidget_process_2->setEnabled(false);          
        }
    }
}

void MainWindow::on_radioButton_img2_clicked(bool checked)
{
    if(checked == true)
    {
        if(selectImg == 1)
        {            
            if(cap2.isOpened())
                cap2.release();

            selectImg = 2;

            emit qInputImageReady();
            cout<<"image 2 selected"<<endl;
            ui->label_img1->setEnabled(false);
            ui->label_img2->setEnabled(true);
            ui->listWidget_process->setEnabled(false);
            ui->listWidget_process_2->setEnabled(true);
        }
    }
}



void MainWindow::on_pushButton_stitch_clicked()
{
    vector<cv::Mat> vImg;
    cv::Mat rImg;

    vImg.push_back(procEng->get_processedImg());
    vImg.push_back(procEng2->get_processedImg());

    cv::Stitcher stitcher = cv::Stitcher::createDefault();
    cv::Stitcher::Status status = stitcher.stitch( vImg, rImg);

    if(cv::Stitcher::OK == status)
        show_cv("panorama", rImg, delay);
    else
        cout<<"panorama failed"<<endl;

}

void MainWindow::on_pushButton_match_clicked()
{
    cv::Mat img1 = procEng->get_originalImg();
    cv::Mat img2 = procEng2->get_originalImg();
//    cv::Mat outImg1, outImg2;
    vector<cv::KeyPoint> keypts1, keypts2;

    QString method1, method2;
    procEng->getKeypoints(keypts1, method1);
    procEng2->getKeypoints(keypts2, method2);
    cout<<method1.toStdString()<<endl;
    cv::Mat descriptors1, descriptors2;

    if( method1.compare("SURF") == 0 && method2.compare("SURF") == 0)
    {
//        cv::Ptr<FeatureDetector> detector = SurfFeatureDetector::create( 2500 );
        cv::Ptr<SURF> descFeat = SURF::create();
        descFeat->compute(img1, keypts1, descriptors1);
        descFeat->compute(img2, keypts2, descriptors2);
    }

    if( method1.compare("SIFT") == 0 && method2.compare("SIFT") == 0)
    {
//        cv::Ptr<FeatureDetector> detector = SiftFeatureDetector:
        cv::Ptr<SIFT> descFeat = SIFT::create();
        descFeat->compute(img1, keypts1, descriptors1);
        descFeat->compute(img2, keypts2, descriptors2);
    }

//    if( method.compare("FAST") == 0 )
//    {

////        Check cookbook for FAST and apply also to Harris
//    }

    if(keypts1.size() != 0 && keypts2.size() != 0)
    {
        BFMatcher matcher(NORM_L2);
//        vector<cv::DMatch> matches;
        matcher.match(descriptors1,descriptors2, matches);

        nth_element(matches.begin(), matches.begin()+24, matches.end());
        matches.erase(matches.begin()+25, matches.end());

        Mat imageMatches;
        drawMatches(img1, keypts1, img2, keypts2, matches, imageMatches, Scalar(255,255,255));

        namedWindow("Matched");
        imshow("Matched", imageMatches);

        cv::waitKey(delay);
    }


}


void MainWindow::on_pushButton_FundMat_clicked()
{
    vector<cv::KeyPoint> keypts1, keypts2;
    QString method1, method2;
    procEng->getKeypoints(keypts1, method1);
    procEng2->getKeypoints(keypts2, method2);

    vector<Point2f> imgPts1, imgPts2;
    for(unsigned int i=0; i<matches.size(); i++)
    {
        imgPts1.push_back(keypts1[matches[i].queryIdx].pt);
        imgPts2.push_back(keypts2[matches[i].trainIdx].pt);
    }

    int methodFM;
    if(ui->radioButton_7pts->isChecked())
        methodFM = cv::FM_7POINT;
    if(ui->radioButton_8pts->isChecked())
        methodFM = cv::FM_8POINT;
    if(ui->radioButton_ransac->isChecked())
        methodFM = cv::FM_RANSAC;

    F = findFundamentalMat(imgPts1, imgPts2, methodFM, 0.1, 0.99);
    cout<<F<<endl;
}

void MainWindow::on_pushButton_epipol_clicked()
{
    cv::Mat img1 = procEng->get_originalImg();
    cv::Mat img2 = procEng2->get_originalImg();

    cv::Mat outImg(img1.rows, img1.cols*2, CV_8UC3);
    cv::Rect rect1(0,0, img1.cols, img1.rows);
    cv::Rect rect2(img1.cols, 0, img1.cols, img1.rows);

    cv::Mat imgROI1(outImg, rect1);
    cv::Mat imgROI2(outImg, rect2);
    img1.copyTo(imgROI1);
    img2.copyTo(imgROI2);

    std::vector< cv::Vec<double,3> > epilines1, epilines2;
    std::vector< cv::Point_<double> >points1, points2;

    vector<cv::KeyPoint> keypoints1, keypoints2;
    QString method;
    procEng->getKeypoints(keypoints1, method);
    procEng2->getKeypoints(keypoints2, method);

    for(unsigned int i=0; i<matches.size(); i++)
    {
        points1.push_back(keypoints1[matches[i].queryIdx].pt);
        points2.push_back(keypoints2[matches[i].trainIdx].pt);
    }

    cv::computeCorrespondEpilines(points1, 1, F, epilines1);
    cv::computeCorrespondEpilines(points2, 2, F, epilines2);

    cv::RNG rng(0);
    float inlierDistance = 5;

    for(size_t i = 0; i<points1.size(); i++)
    {
        if(inlierDistance>0)
        {
            if(distancePointLine(points1[i], epilines2[i]) > inlierDistance || distancePointLine(points2[i], epilines1[i]) > inlierDistance)
                continue;
        }

        cv::Scalar color(rng(256),rng(256),rng(256));
        cv::line(outImg(rect2),
              cv::Point(0,-epilines1[i][2]/epilines1[i][1]),
              cv::Point(img1.cols,-(epilines1[i][2]+epilines1[i][0]*img1.cols)/epilines1[i][1]),
              color);
            cv::circle(outImg(rect1), points1[i], 3, color, -1, CV_AA);

        cv::line(outImg(rect1),
              cv::Point(0,-epilines2[i][2]/epilines2[i][1]),
              cv::Point(img2.cols,-(epilines2[i][2]+epilines2[i][0]*img2.cols)/epilines2[i][1]),
              color);
            cv::circle(outImg(rect2), points2[i], 3, color, -1, CV_AA);

    }
    cv::imshow("Epipolar lines", outImg);
    cv::waitKey(1);
}

void MainWindow::on_pushButton_calib_clicked()
{
    calibrationEnabled = !calibrationEnabled;
    if(calibrationEnabled == true)
    {
        object_points.clear();
        image_points.clear();
        nCorners = 0;
    }
}

bool MainWindow::findCorners(vector<vector<Point3f> > &object_points, vector<vector<Point2f> > &image_points, cv::Mat image)
{
    int numCornersHor = 9;
    int numCornersVer = 6;

    int numSquares = numCornersHor * numCornersVer;
    Size board_sz = Size(numCornersHor, numCornersVer);

    vector<cv::Point2f> corners;
    int successes=0;

    Mat gray_image;

    vector<cv::Point3f> obj;
    for(int j=0;j<numSquares;j++)
        obj.push_back(cv::Point3f(j/numCornersHor, j%numCornersHor, 0.0f));

    cv::cvtColor(image, gray_image, CV_BGR2GRAY);
    bool found = cv::findChessboardCorners(image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS );

    if(found)
    {
        cv::cornerSubPix(gray_image, corners, cv::Size(11, 11), cv::Size(-1,-1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.01));
        cv::drawChessboardCorners(gray_image, board_sz, corners, found);
    }

    imshow("calibration", gray_image);
    cv::waitKey(1);

    if(found!=0)
    {
        image_points.push_back(corners);
        object_points.push_back(obj);
        cout<<"Snap stored!"<<endl;
    }

    return found;
}

cv::Mat MainWindow::calibrateAndUndistort(const vector<vector<Point3f> > &object_points, const vector<vector<Point2f> > &image_points, const cv::Mat &image)
{
    cv::Mat intrinsic = cv::Mat(3, 3, CV_32FC1);
    cv::Mat distCoeffs;
    vector<cv::Mat> rvecs;
    vector<cv::Mat> tvecs;

    intrinsic.ptr<float>(0)[0] = 1;
    intrinsic.ptr<float>(1)[1] = 1;

    cv::calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);

    cv::Mat imageUndistorted;
    cv::undistort(image, imageUndistorted, intrinsic, distCoeffs);

    return imageUndistorted;
}

void MainWindow::on_pushButton_homo_clicked()
{
    vector<cv::KeyPoint> keypts1, keypts2;
    QString method1, method2;
    procEng->getKeypoints(keypts1, method1);
    procEng2->getKeypoints(keypts2, method2);


    vector<Point2f> imgPts1, imgPts2;
    for(unsigned int i=0; i<matches.size(); i++)
    {
        imgPts1.push_back(keypts1[matches[i].queryIdx].pt);
        imgPts2.push_back(keypts2[matches[i].trainIdx].pt);
    }

    int methodHM;
    if(ui->radioButton_allPts->isChecked())
        methodHM = 0;
    if(ui->radioButton_ransacH->isChecked())
        methodHM = CV_RANSAC;
    if(ui->radioButton_ransac->isChecked())
        methodHM = CV_LMEDS;


    std::vector<uchar>	inliers(imgPts1.size(),0);
    H = cv::findHomography(imgPts1, imgPts2, inliers, methodHM );
}


void MainWindow::on_pushButton_warp_clicked()
{
    cv::Mat imOut;
    cv::warpPerspective(procEng->get_processedImg(), imOut, H, procEng2->get_originalImg().size());

    show_cv("warped", imOut, delay);
}
