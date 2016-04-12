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
    QObject::connect(procEng,SIGNAL(ImgReadyIn()),this,SLOT(Display_inImg()));
    QObject::connect(procEng, SIGNAL(ImgReadyOut()), this, SLOT(Display_outImg()));
    QObject::connect(procEng, SIGNAL(ImgReadyOut()), this, SLOT(Disable_widgets()));
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
    show_cv(inFrame,procEng->get_originalImg(),delay);
    cv::moveWindow(inFrame,screen.width()/2,0);
}

void MainWindow::Display_outImg()
{
    show_cv(outFrame,procEng->get_processedImg(),delay);
    cv::moveWindow(outFrame,screen.width()/2,screen.height()/2);
}

void MainWindow::Disable_widgets()
{
    int type_img = procEng->get_processedImg().type();
    cout<<type_img<<endl;

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
    I = imread(filename.toStdString(),CV_LOAD_IMAGE_COLOR);
    procEng->loadImg(filename);
}

void MainWindow::on_pushButton_webcam_clicked()
{
    Mat frame,flipped,resized,img,blurred,edges;
    int ch,delay=10;

    while(cap.read(frame))
    {
        ch = show_cv("Input", frame, delay);
//        Canny(frame, edges, 50, 150);
//        ch = show_cv("Canny",edges,delay);
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
    procEng->addProcess("Reset");
}


/**
 * Slots called by buttons applying a process on the image
 */

void MainWindow::on_pushButton_Blur_clicked()
{
    procEng->addProcess("Blur");
}

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

void MainWindow::on_listWidget_activated(const QModelIndex &index)
{
    switch(index.row())
    {
    case 0: procEng->addProcess("Erode");
        break;
    case 1: procEng->addProcess("Dilate");
        break;
    case 2: procEng->addProcess("Open");
        break;
    case 3: procEng->addProcess("Close");
        break;
    default: break;
    }
}

void MainWindow::on_pushButton_sp_clicked()
{
    procEng->addProcess("SP");
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
    procEng->displayHist();
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
    procEng->addProcess("Sobel");
}

void MainWindow::on_pushButton_lapl_clicked()
{
    procEng->addProcess("Laplacian");
}

void MainWindow::on_pushButton_kernel_clicked()
{
    procEng->addProcess("Kernel");
}

void MainWindow::on_pushButton_canny_clicked()
{
    procEng->addProcess("Canny");
}

void MainWindow::on_pushButton_circles_clicked()
{
    procEng->addProcess("Circles");
}

void MainWindow::on_pushButton_lines_clicked()
{
    procEng->addProcess("Lines");
}

void MainWindow::on_pushButton_harris_clicked()
{
    procEng->addProcess("Harris");
}

void MainWindow::on_pushButton_surf_clicked()
{
    procEng->addProcess("SURF");
}

void MainWindow::on_pushButton_sift_clicked()
{
    procEng->addProcess("SIFT");
}

void MainWindow::on_pushButton_fast_clicked()
{
    procEng->addProcess("FAST");
}
