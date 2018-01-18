#ifndef _LIBPEEPHOLE_TCPHANDLER_H_
#define _LIBPEEPHOLE_TCPHANDLER_H_

#include <uv.h>
#include "ph_def.h"

PH_NAMESPACE_DECL_BEGIN
class TcpHandler
{
public:
    TcpHandler(uv_loop_t *loop);
    ~TcpHandler();

    int Accept(uv_stream_t *server); 
private:
    uv_loop_t *loop_;
    uv_tcp_t stream_;
};
PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_TCPHANDLER_H_

