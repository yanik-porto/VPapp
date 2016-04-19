#include "processengine.h"

processEngine::processEngine(QObject *parent) : QObject(parent)
{

}

processEngine::~processEngine()
{

}

void processEngine::loadImg(const cv::Mat &matImg)
{
    //inI = cv::imread(filename.toStdString());
    inI = matImg;
    outI = inI.clone();
    process();
    ImgReadyIn();
    ImgReadyOut();
}

void processEngine::changeToBGR()
{
    if(outI.type()==0)
    {
        cv::cvtColor(outI, outI, CV_GRAY2BGR);
    }
}

void processEngine::addProcess(const QString &str, va_list args)
{

//    va_list args;
//    va_start(args, str);

    ipmethod *method;



    if(str.compare("flip")==0)
    {
      int flipCode = va_arg( args, int);
      method = new flipImg(flipCode);
    }

    if(str.compare("blur")==0)
    {
      int sizeKernel = va_arg( args, int );
      method = new Filters(str, sizeKernel);

    }

    if(str.compare("sharpen")==0)
    {
        int sizeKernel = va_arg( args, int );
        method = new Filters(str, sizeKernel);
    }

    if(str.compare("erode")==0)
    {
        int sizeKernel = va_arg( args, int );
        method = new Filters(str, sizeKernel);
    }

    if(str.compare("dilate")==0)
    {
        int sizeKernel = va_arg( args, int );
        method = new Filters(str, sizeKernel);
    }

    if(str.compare("open")==0)
    {
        int sizeKernel = va_arg( args, int );
        method = new Filters(str, sizeKernel);
    }

    if(str.compare("close")==0)
    {
        int sizeKernel = va_arg( args, int );
        method = new Filters(str, sizeKernel);
    }

    if(str.compare("SP")==0)
    {
        double rate = va_arg( args, double );
        method = new SandP( rate );
    }

    if(str.compare("logo")==0)
    {
        QString filename = QFileDialog::getOpenFileName();

        if( filename.compare("") != 0 )
            method = new AddLogo( filename );
    }

    if(str.compare("invert")==0)
    {
        method= new colorBasics( str );
    }

    if(str.compare("equalize")==0)
    {
        method= new colorBasics( str );
    }

    if(str.compare("Resize")==0)
    {
        int height = va_arg( args, int );
        int width = va_arg( args, int );
        method = new resizeImg( width, height);
    }

    if(str.compare("sobel")==0)
    {
        method = new Derivatives(str);
    }

    if(str.compare("laplacian")==0)
    {
        method = new Derivatives(str);
    }

    if(str.compare("canny")==0)
    {
        method = new Derivatives(str);
    }

    if(str.compare("circles")==0)
    {
        int param2 = va_arg( args, int );
        method = new HoughTrans( str, param2 );
     }

    if(str.compare("lines")==0)
    {
        int thresh = va_arg( args, int );
        method = new HoughTrans( str, thresh );
    }

    if(str.compare("Harris")==0)    //Do non maximal suppression
    {
        int thresh = va_arg( args, int );
        method = new FeaturesPts( str, thresh );
    }

    if(str.compare("SURF")==0)
    {
        int minHessian = va_arg( args, int );
        method = new FeaturesPts( str, minHessian );
    }

    if(str.compare("SIFT")==0)
    {
        int nPts = va_arg( args, int );
        method = new FeaturesPts( str, nPts );
    }

    if(str.compare("FAST")==0)
    {
        int thresh = va_arg( args, int );
        method = new FeaturesPts( str, thresh );
    }

    ipmethodList.push_back(method);

    va_end(args);

    process();

}

void processEngine::computeHist(cv::Mat histImage[3])
{
    ///initialize the histograms for B, G and R
   int hist_w = 512; int hist_h = 400;

   histImage[0] = cv::Mat( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );
   histImage[1] = cv::Mat( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );
   histImage[2] = cv::Mat( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );

   ///Fill the matrices
   getHistogram(outI, histImage);
}

void processEngine::process()
{

    outI = inI.clone();
    vector<ipmethod*>::iterator it;
    for ( it=ipmethodList.begin(); it!=ipmethodList.end(); it++)
    {
        (*it)->process( outI, outI );
        changeToBGR();
    }

    emit ImgReadyOut();
}

void processEngine::reset()
{
    outI = inI.clone();
    if (!ipmethodList.empty()){
        ipmethodList.clear();
        process();
    }
}
