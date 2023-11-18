#ifndef SST_EXEPTION_H
#define SST_EXEPTION_H

#include <QException>


class SstException : public QException
{
public:
    SstException(QString msg)
        : msg_(msg.toStdString())
    {}

    const char* what() const noexcept override
    {
        return msg_.c_str();
    }

private:
    std::string msg_;

};


#endif // SST_EXEPTION_H
