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
    cap(0),
    delay(10),
    selectImg(1)
{
    ui->setupUi(this);
    screen = QApplication::desktop()->screenGeometry();cout<<screen.height()<<endl;
    this->setGeometry(0,0,screen.width()/2-68,screen.height());

    procEng = new processEngine;
    procEng2 = new processEngine;

    QObject::connect(this,SIGNAL(qInputImageReady()),this,SLOT(Display_inImg()));
    QObject::connect(procEng, SIGNAL(ImgReadyOut()), this, SLOT(Display_outImg()));
    QObject::connect(procEng, SIGNAL(ImgReadyOut()), this, SLOT(Disable_widgets()));

    QObject::connect(procEng2, SIGNAL(ImgReadyOut()), this, SLOT(Display_outImg()));

    cv::moveWindow(outFrame,screen.width()/2,0);

    ui->listWidget_process->setEnabled(true);
    ui->listWidget_process_2->setEnabled(false);
    ui->label_img1->setEnabled(true);
    ui->label_img2->setEnabled(false);
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
        ui->label_inputI->setPixmap(QPixmap::fromImage(originalImg.scaled(ui->label_inputI->size(), Qt::KeepAspectRatio, Qt::FastTransformation)));
        break;
    case 2:
        ui->label_inputI->setPixmap(QPixmap::fromImage(originalImg2.scaled(ui->label_inputI->size(), Qt::KeepAspectRatio, Qt::FastTransformation)));
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
        break;
    case 2: show_cv(outFrame2, procEng2->get_processedImg(), delay);
        break;
    default:
        break;
    }


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
}

void MainWindow::on_pushButton_sift_clicked()
{
    int nPts = ui->horizontalSlider_SIFTThresh->value();
    sendProcessRequest( selectImg, "SIFT", nPts );
    updateListProcess("SIFT detector " + QString::number(nPts) );
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
