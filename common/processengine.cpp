#include "processengine.h"

#include <opencv2/imgproc.hpp>
/**
 * constructor and destructor of the class
 */
processEngine::processEngine(QObject *parent) : QObject(parent),
    featPts(nullptr),
    channelsFilter(nullptr)
{
//    featPts = new FeaturesPts();
}

processEngine::~processEngine()
{
    this->reset();
}

/**
 * enter image inside the class
 */
void processEngine::loadImg(const cv::Mat &matImg)
{
    inI = matImg;
    outI = inI.clone();
    process();
    ImgReadyIn();
    ImgReadyOut();
}

/**
 * convert the image to BGR
 * called everytime an image is ready to output so that we can apply any kind of method on it after
 */
void processEngine::changeToBGR()
{
    if(outI.type()==0)
    {
        cv::cvtColor(outI, outI, cv::COLOR_GRAY2BGR);
    }
}

/**
 * main function where all the process are applied
 */
void processEngine::addProcess(const QString &str, va_list args)
{

    //Instantiate empty method
    ipmethod *method = nullptr;

    //Check which kind of method to add to the list (downgrade to mother class)
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
        //Ask to open a file here
        QString filename = QFileDialog::getOpenFileName();

        if( filename.compare("") != 0 )
            method = new AddLogo( filename );
    }

    if(str.compare("invert")==0)
    {
        method= new colorBasics( str );
    }

    if(str.compare("2rgb")==0)
    {
        method = new colorBasics( str );
    }

    if(str.compare("2hsv")==0)
    {
        method = new colorBasics( str );
    }

    if(str.compare("2gray")==0)
    {
        method = new colorBasics( str );
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

    if(str.compare("contours")==0)
    {
        method = new HoughTrans( str );
    }

    if(str.compare("Harris")==0)
    {
        int thresh = va_arg( args, int );
        method = new FeaturesPts( str, thresh );
    }

    if(str.compare("FAST")==0)
    {
        int thresh = va_arg( args, int );
        method = new FeaturesPts( str, thresh );
    }

    if(str.compare("SURF")==0)
    {
        //Store the method first in a variable so that we can know if descriptors have already been computed
        int minHessian = va_arg( args, int );
        featPts = new FeaturesPts( str, minHessian );
        method = featPts;
    }

    if(str.compare("SIFT")==0)
    {
        int nPts = va_arg( args, int );
        featPts = new FeaturesPts( str, nPts );
        method = featPts;
    }

    if(str.compare("update_channels")==0)
    {

        int min1 = va_arg( args, int );
        int min2 = va_arg( args, int );
        int min3 = va_arg( args, int );
        int max1 = va_arg( args, int );
        int max2 = va_arg( args, int );
        int max3 = va_arg( args, int );
        int mode = va_arg( args, int );

        if (channelsFilter == nullptr) {
            channelsFilter = new ChannelsFilter();
            method = channelsFilter;
        }

        channelsFilter->add(min1, min2, min3, max1, max2, max3, mode);
    }

    if (method != nullptr) {
        ipmethodList.push_back(method);
    }

    va_end(args);

    process();

}

/**
 * compute the histogram of every channel and store the in the input matrices
 */
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

/**
 * function which executes all methods in the list
 */
void processEngine::process()
{

    //Sweep all the list and process each item
    outI = inI.clone();
    vector<ipmethod*>::iterator it;
    for ( it=ipmethodList.begin(); it!=ipmethodList.end(); it++)
    {
        (*it)->process( outI, outI );
        changeToBGR();
    }

    emit ImgReadyOut();
}

/**
 * reset the list of processes
 */
void processEngine::reset()
{
    outI = inI.clone();
    for ( auto it=ipmethodList.begin(); it!=ipmethodList.end();) {
        it = ipmethodList.erase(it);
    }
    process();

    featPts = nullptr;
    channelsFilter = nullptr;
}

/**
 * remove one method of the list giving its index
 */
void processEngine::removeMethod(const int &ind)
{
    ipmethod *pointer = *(ipmethodList.begin() + ind);
    if (pointer == featPts) {
        featPts = nullptr;
    }
    if (pointer == channelsFilter) {
        channelsFilter = nullptr;
    }
    delete pointer;

    ipmethodList.erase( ipmethodList.begin() + ind );
    process();
}

/**
 * return the keypoints if they have been computed previously
 */
void processEngine::getKeypoints(vector<cv::KeyPoint> &keypoints, QString &method)
{
    if(featPts)
    {
        cout<<"Features detected"<<endl;
        keypoints = featPts->get_keypoints();
        method = featPts->get_method();
    }

    else
        cout<<"No features detected"<<endl;
}

const std::vector<ipmethod *> processEngine::getProcessList() const
{
    return ipmethodList;
}
