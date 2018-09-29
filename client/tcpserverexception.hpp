#ifndef TCPSERVEREXCEPTION_HPP
#define TCPSERVEREXCEPTION_HPP

#include <QSharedMemory>
#include <QException>
class TcpServerException : public QException
{
public:
    explicit TcpServerException(QString const& message);
    TcpServerException(TcpServerException const& other) = default;
    QString errorMessage() const;

    void raise() const;
    TcpServerException* clone() const;


private:
    QString const message;
};

#endif // TCPSERVEREXCEPTION_HPP
