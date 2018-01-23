#ifndef _LIBPEEPHOLE_TCPHANDLER_H_
#define _LIBPEEPHOLE_TCPHANDLER_H_

#include <uv.h>
#include "ph_decl.h"
#include "iobuffer.h"

PH_NAMESPACE_DECL_BEGIN
class IOAdapter;
class TcpHandler
{
public:
    TcpHandler(uv_loop_t *loop, IOAdapter *adpt);
    ~TcpHandler();

    static void alloc_cb(uv_handle_t* handle, size_t size, uv_buf_t* buf);
    static void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
    static void conn_getaddrinfo_done(uv_getaddrinfo_t *req, int status, struct addrinfo *ai);
    static void connect_cb(uv_connect_t* req, int status);

    int Connect(const char *host, uint16_t port);
    int Accept(uv_stream_t *server); 
    int Close();
private:
    void Detach();

    uv_loop_t *loop_;
    IOBuffer rxbuf_;

    uv_connect_t connect_req_; 
    uv_getaddrinfo_t addr_req_;

    uv_tcp_t stream_;
    IOAdapter *adapter_;
};
PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_TCPHANDLER_H_

