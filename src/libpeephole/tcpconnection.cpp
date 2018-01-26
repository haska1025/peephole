#include "tcpconnection.h"

TcpConnection::TcpConnection(IOThread *iothr, int mode):iothr_(iothr),mode_(mode)
{
}
TcpConnection::~TcpConnection()
{
}

int TcpConnection::Open(const char *host, uint16_t port)
{
    if (!handler_){
        handler_ = new TcpHandler(iothr_->uv_loop(), this); 
        if (!handler_)
            return -12;//No more memory
    }
    return handler_->Connect(host, port);
}

IOAdapter * TcpConnection::OnAccept()
{
    return new TcpConnection();
}
void TcpConnection::OnConnect()
{
}
int TcpConnection::OnRecv(IOBuffer *buf)
{
}
int TcpConnection::OnSend()
{
}
void TcpConnection::OnClose()
{
}

