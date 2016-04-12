#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    inFrame("Input"),
    outFrame("Output"),
    cap(0),
    delay(10)
{
    ui->setupUi(this);

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

int MainWindow::show_cv(const String &winname,const Mat &image, const int &delay)
{
    cv::namedWindow(winname);
    cv::imshow(winname, image);
    int ch;
    ch = cv::waitKey(delay);
    return ch;
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

void MainWindow::Display_inImg()
{
    show_cv(inFrame,procEng->get_originalImg(),delay);
    cv::moveWindow(inFrame,QApplication::desktop()->screenGeometry().width()/2,0);
}

void MainWindow::Display_outImg()
{
    show_cv(outFrame,procEng->get_processedImg(),delay);
    cv::moveWindow(outFrame,QApplication::desktop()->screenGeometry().width()/2,QApplication::desktop()->screenGeometry().height()/2);
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

void MainWindow::on_pushButton_Blur_clicked()
{
    procEng->addProcess("Blur");
}

void MainWindow::on_pushButton_flip_clicked()
{
    procEng->addProcess("Flip");
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
    procEng->addProcess("Resize");
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
