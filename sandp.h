#ifndef SANDP_H
#define SANDP_H

#include "ipmethod.h"

class SandP : public ipmethod
{
private:
    double rate;

public:
    SandP();
    SandP( const double & );
    SandP( const SandP &);
    ~SandP();

    void set_rate(const double &);

    void salt(cv::Mat &, cv::Mat &, int);
    void pepper(cv::Mat &, cv::Mat &,int);

    void process(cv::Mat &, cv::Mat &);




};

#endif // SANDP_H
