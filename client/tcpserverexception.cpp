#include "tcpserverexception.hpp"

TcpServerException::TcpServerException(const QString &message)
    :message{message}
{

}

QString TcpServerException::errorMessage() const{
    return this->message;
}

void TcpServerException::raise() const{
    throw *this;
}

TcpServerException* TcpServerException::clone() const{
    return new TcpServerException(*this);
}

