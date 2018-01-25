#ifndef _LIBPEEPHOLE_TCPHANDLER_H_
#define _LIBPEEPHOLE_TCPHANDLER_H_

#include <uv.h>
#include "ph_decl.h"
#include "iobuffer.h"
#include "iohandler.h"

PH_NAMESPACE_DECL_BEGIN
class IOAdapter;
class TcpHandler : public IOHandler
{
public:
    TcpHandler(uv_loop_t *loop, IOAdapter *adpt);
    ~TcpHandler();

    static void alloc_cb(uv_handle_t* handle, size_t size, uv_buf_t* buf);
    static void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
    static void write_cb(uv_write_t* req, int status);
    static void conn_getaddrinfo_done(uv_getaddrinfo_t *req, int status, struct addrinfo *ai);
    static void connect_cb(uv_connect_t* req, int status);

    int Connect(const char *host, uint16_t port);
    int Send(const void *buff, uint32_t len);
    int Accept(uv_stream_t *server); 
    int Close();
private:
    void SaveUnsentData(uv_buf_t *sentbuf, uint32_t sentsize);
    void ResetUnsentBuf();
    int WantWrite();
    void Detach();

    IOBuffer rxbuf_;

    uv_connect_t connect_req_; 
    uv_getaddrinfo_t addr_req_;

    uv_buf_t unsent_buf_;
    uv_write_t send_req_; 
};
PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_TCPHANDLER_H_

