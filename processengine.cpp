#include "processengine.h"

processEngine::processEngine(QObject *parent) : QObject(parent)
{
    QObject::connect(this,SIGNAL(ImgReadyOut()),this,SLOT(changeToGray()));
}

processEngine::~processEngine()
{

}

void processEngine::loadImg(const QString &filename)
{
    inI = cv::imread(filename.toStdString());
    outI = inI.clone();
    ImgReadyIn();
    ImgReadyOut();
}

void processEngine::changeToGray()
{
    if(outI.type()==0)
    {
        cv::cvtColor(outI, outI, CV_GRAY2BGR);
    }
}

void processEngine::addProcess(const QString &str, ...)
{

    va_list args;
    va_start(args, str);

    //Maybe move later on
    if(str.compare("Reset")==0)
    {
        outI = inI.clone();
    }

    if(str.compare("Blur")==0)
    {
      int sizeKernel = va_arg( args, int );
      cv::blur( outI, outI, cv::Size( sizeKernel, sizeKernel ) );
    }

    if(str.compare("Flip")==0)
    {
      int flipCode = va_arg( args, int );
      cv::flip( outI, outI, flipCode );
    }

    if(str.compare("Erode")==0)
    {
        int sizeElmt = va_arg( args, int );
        cv::Mat	element( sizeElmt, sizeElmt, CV_8U,cv::Scalar(1) );
        cv::erode( outI, outI, element );
    }

    if(str.compare("Dilate")==0)
    {
        int sizeElmt = va_arg( args, int );
        cv::Mat	element( sizeElmt, sizeElmt, CV_8U, cv::Scalar(1) );
        cv::dilate(outI,outI,element);
    }

    if(str.compare("Open")==0)
    {
        int sizeElmt = va_arg( args, int );
        cv::Mat	element( sizeElmt, sizeElmt, CV_8U,cv::Scalar(1) );
        cv::morphologyEx(outI,outI,cv::MORPH_OPEN,element);
    }

    if(str.compare("Close")==0)
    {
        int sizeElmt = va_arg( args, int );
        cv::Mat	element( sizeElmt, sizeElmt, CV_8U,cv::Scalar(1) );
        cv::morphologyEx(outI,outI,cv::MORPH_CLOSE,element);
    }

    if(str.compare("SP")==0)
    {
        sp_noise(outI, outI);
    }

    if(str.compare("Logo")==0)
    {
        QString filename = QFileDialog::getOpenFileName();
        addLogo(filename,outI,outI);
    }

    if(str.compare("Invert")==0)
    {
        outI = cv::Scalar::all(255) - outI;
    }

    if(str.compare("Equalize")==0)
    {
        vector<cv::Mat> bgr_planes;
        cv::split(outI, bgr_planes);
        cv::equalizeHist(bgr_planes[0],bgr_planes[0]);
        cv::equalizeHist(bgr_planes[1],bgr_planes[1]);
        cv::equalizeHist(bgr_planes[2],bgr_planes[2]);
        cv::merge(bgr_planes,outI);
    }

    if(str.compare("Resize")==0)
    {
        int height = va_arg( args, int );
        int width = va_arg( args, int );
        cv::Size size( width, height );
        cv::resize(outI,outI,size);
    }

    if(str.compare("Sobel")==0)
    {

        /// Convert image to gray
        cvtColor( outI, outI, CV_BGR2GRAY );

        /// Generate gradient images and set parameter of the depth to 16S
        cv::Mat grad_x, grad_y;
        int ddepth = CV_16S;

        ///Apply sobel
        cv::Sobel(outI, grad_x,  ddepth,1,0);
        cv::Sobel(outI, grad_y,  ddepth,0,1);

        ///convert our partial results back to CV_8U
        cv::convertScaleAbs(grad_x,grad_x);
        cv::convertScaleAbs(grad_y,grad_y);

        ///Combine both gradients. Total Gradient (approximate)
        cv::addWeighted(grad_x, 0.5, grad_y, 0.5, 0, outI);
    }

    if(str.compare("Laplacian")==0)
    {
        /// Convert image to gray
        cvtColor( outI, outI, CV_BGR2GRAY );

        ///Set parameter of the depth to 16S
        int ddepth = CV_16S;

        ///Generate intermediate image and apply Laplacian
        cv::Mat lapl16;
        cv::Laplacian(outI, lapl16, ddepth,3);

        ///convert to CV_8U
        cv::convertScaleAbs(lapl16, outI);
    }

    if(str.compare("Kernel")==0)
    {
        int center = va_arg( args, int );
        float border = -(center-1)/4;
        int kernel_size = 3;
        cv::Mat kernel = ( cv::Mat_<double>( kernel_size, kernel_size ) << 0, border, 0, border, center, border, 0, border, 0 );
        cv::filter2D( outI, outI, -1, kernel );
    }

    if(str.compare("Canny")==0)
    {
        /// Convert image to gray
        cvtColor( outI, outI, CV_BGR2GRAY );

        /// Set the parameters
        int lowThresh = 100;
        int ratio = 3;

        /// Apply Canny
        cv::Mat detectedEdges;
        cv::Canny(outI,outI,lowThresh,lowThresh*ratio);
    }

    if(str.compare("Circles")==0)
    {
        ///Convert image to gray
        cv::Mat outI_gray;
        cvtColor(outI, outI_gray, CV_BGR2GRAY);

        ///Create vectors to store x,y and r
        vector<cv::Vec3f> circles;

        ///Apply the hough transform to find circles
        int param2 = va_arg( args, int );
        cv::HoughCircles(outI_gray, circles, CV_HOUGH_GRADIENT, 1, outI_gray.rows/8, 200, param2, 0, 0); //Play with parameter 2
        cout<<circles.size()<<endl;
        ///Draw detected circles
        for( size_t i = 0; i < circles.size(); i++ )
        {
            cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            cv::circle( outI, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
            // circle outline
            cv::circle( outI, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
         }

     }

    if(str.compare("Lines")==0)
    {
        ///Convert image to gray
        cv::Mat outI_gray;
        cvtColor(outI, outI_gray, CV_BGR2GRAY);

        ///Create vector to store lines
        vector<cv::Vec4i> lines;

        ///Apply hough transform
        int thresh = va_arg( args, int );
        cv::HoughLinesP( outI_gray, lines, 1, CV_PI/180, thresh, 50, 10 );
        cout<<lines.size()<<endl;

        ///Draw the detected lines
        for( size_t i = 0; i < lines.size(); i++ )
        {
          cv::Vec4i l = lines[i];
          cv::line( outI, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 1, CV_AA);
        }
    }

    if(str.compare("Harris")==0)    //Do non maximal suppression
    {
        ///Convert image to gray
        cv::Mat outI_gray, corners, corners_scaled;
        cvtColor(outI, outI_gray, CV_BGR2GRAY);

        ///Detector Parameters
        int blockSize = 2;
        int apertureSize = 3;
        double k = 0.04;
        int thresh = va_arg( args, int );

        ///Detect corners
        cv::cornerHarris( outI_gray, corners, blockSize, apertureSize, k, cv::BORDER_DEFAULT );

        /// Normalizing
        cv::normalize( corners, corners, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );
        cv::convertScaleAbs( corners, corners_scaled );

        /// Drawing a circle around corners
        for( int j = 0; j < corners.rows ; j++ )
           { for( int i = 0; i < corners.cols; i++ )
                {
                  if( (int) corners.at<float>(j,i) > thresh )
                    {
                     cv::circle( outI, cv::Point( i, j ), 5,  cv::Scalar(0,0,255), 2, 8, 0 );
                    }
                }
           }
    }

    if(str.compare("SURF")==0)
    {
        int minHessian = va_arg( args, int );
        cv::Ptr<SURF> detector = SURF::create( minHessian );

        vector<cv::KeyPoint> keypoints;
        detector->detect( outI, keypoints );

        cv::drawKeypoints(outI, keypoints, outI, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT );

    }

    if(str.compare("SIFT")==0)
    {
        int nPts = va_arg( args, int );
        cv::Ptr<cv::Feature2D> f2d = SIFT::create( nPts );

        vector<cv::KeyPoint> keypoints;
        f2d->detect( outI, keypoints );

        cv::drawKeypoints( outI, keypoints, outI, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT );

    }

    if(str.compare("FAST")==0)
    {
        cv::Mat outI_gray;
        cv::cvtColor(outI, outI_gray, CV_BGR2GRAY);

        int thresh = va_arg( args, int ); //9;
        vector<cv::KeyPoint> keypoints;
        cv::FAST(outI_gray, keypoints, thresh, true);

        cv::drawKeypoints( outI, keypoints, outI, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT );
    }

    ImgReadyOut();

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
