#ifndef SST_EXEPTION_H
#define SST_EXEPTION_H

#include <QException>


class SstException : public QException
{
public:
    SstException(QString msg)
        : msg(msg.toStdString())
    {}

    const char* what() const noexcept override
    {
        return msg.c_str();
    }

private:
    std::string msg;

};


#endif // SST_EXEPTION_H
