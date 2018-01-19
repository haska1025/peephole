#ifndef _LIBPEEPHOLE_TCPHANDLER_H_
#define _LIBPEEPHOLE_TCPHANDLER_H_

#include <uv.h>
#include "ph_def.h"

PH_NAMESPACE_DECL_BEGIN
class IOAdapter;
class TcpHandler
{
public:
    TcpHandler(uv_loop_t *loop, IOAdapter *adpt);
    ~TcpHandler();

    static void alloc_cb(uv_handle_t* handle, size_t size, uv_buf_t* buf);
    static void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);

    int Accept(uv_stream_t *server); 
    int Close();
private:
    void Detach();

    uv_loop_t *loop_;
    uv_tcp_t stream_;
    IOAdapter *adapter_;
};
PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_TCPHANDLER_H_

