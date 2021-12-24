#ifndef ADDLOGO_H
#define ADDLOGO_H

#include "ipmethod.h"

class AddLogo : public ipmethod
{
private:
    QString logoPath;

public:
    AddLogo();
    AddLogo( const QString & );
    AddLogo( const AddLogo & );
    ~AddLogo();

    void set_logoPath( const QString & );
    const QString &get_logoPath() const {return logoPath;}

    void process(cv::Mat &, cv::Mat &);

    QString toStr() override;
};

#endif // ADDLOGO_H
