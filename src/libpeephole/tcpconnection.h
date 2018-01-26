#ifndef _LIBPEEPHOLE_TCPCONNECTION_H_
#define _LIBPEEPHOLE_TCPCONNECTION_H_

#include <uv.h>
#include "ph_decl.h"
#include "ioadapter.h"

PH_NAMESPACE_DECL_BEGIN
class IOThread;
class TcpConnection : public IOAdapter
{
public:
    enum conn_mode{
        CM_NONE,
        CM_PASSIVE,
        CM_ACTIVE,
        CM_LISTEN
    };
    TcpConnection(IOThread *iothr, int mode);
    ~TcpConnection();

    int Open(const char *host, uint16_t port);

    // The IOAdapter functions
    virtual IOAdapter * OnAccept();
    virtual void OnConnect();
    virtual int OnRecv(IOBuffer *buf);
    virtual int OnSend();
    virtual void OnClose();

private:
    IOThread *iothr_;
    int mode_;
};

PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_TCPCONNECTION_H_

