#ifndef _LIBPEEPHOLE_TCPLISTENER_H_
#define _LIBPEEPHOLE_TCPLISTENER_H_

#include <uv.h>
#include "ph_decl.h"
#include "iohandler.h"

PH_NAMESPACE_DECL_BEGIN
class IOAdapter;
class TcpListener : public IOHandler
{
public:
    static void on_connection(uv_stream_t *server, int status);

    TcpListener(uv_loop_t *loop, IOAdapter *adpt);
    ~TcpListener();

    int Open(const char *host, unsigned int port);
    int Close();
private:
    uv_tcp_t *accept_;
    int accept_size_;
};
PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_TCPLISTENER_H_

