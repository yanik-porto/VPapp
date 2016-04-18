#include "addlogo.h"

AddLogo::AddLogo()
{

}

AddLogo::AddLogo(const QString &filename)
{
    logoPath = filename;
}

AddLogo::AddLogo(const AddLogo &aL)
{
    logoPath = aL.get_logoPath();
}

AddLogo::~AddLogo()
{

}

void AddLogo::process(cv::Mat &inMat, cv::Mat &outMat)
{
    //Add exception for grayscale image
    cv::Mat logo = cv::imread(logoPath.toStdString());
    int startRow = inMat.rows-logo.rows;
    int startCol = inMat.cols-logo.cols;

    cv::Mat imgROI( outMat,cv::Rect( startCol, startRow, logo.cols, logo.rows ));
    logo.copyTo( imgROI );
}

void AddLogo::set_logoPath(const QString &filename)
{
    logoPath = filename;
}
