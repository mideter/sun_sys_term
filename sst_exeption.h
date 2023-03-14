#ifndef SSTEXEPTION_H
#define SSTEXEPTION_H

#include <QException>

class SstExeption : public QException
{
private:
    QString msg;

public:
    SstExeption(QString msg)
        : msg(msg)
    {}

    const char* what() const noexcept override
    {
        return msg.toStdString().c_str();
    }
};

#endif // SSTEXEPTION_H
