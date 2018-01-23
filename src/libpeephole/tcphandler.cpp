#include <string.h>
#include <stdlib.h>
#include "tcphandler.h"
#include "ioadapter.h"

void TcpHandler::alloc_cb(uv_handle_t* handle, size_t size, uv_buf_t* buf)
{
    int rc = 0;
    TcpHandler *th = (TcpHandler*)handle;

    if (th){
        rc = th->rxbuf_.AdjustUnpublishedCapacity();
        if (rc != 0){
            // Close connection?
            th->Detach();
            return;
        }

        buf->base = (char*)th->rxbuf_.published_pointer(); 
        buf->len = th->rxbuf_.unpublished_size();
    }
}
void TcpHandler::read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    TcpHandler *th = (TcpHandler*)stream;
    if (nread > 0){
        if (th->adapter_){
            th->adapter_->OnRecv(&th->rxbuf_);
        }
    }else if(nread == 0){
        return;
    }else{
        th->Detach();
    }
}
void TcpHandler::conn_getaddrinfo_done(uv_getaddrinfo_t *req, int status, struct addrinfo *ai)
{
    TcpHandler *handler = (TcpHandler*)req->data;
    if (status == 0){
        int rc = 0;
        rc = uv_tcp_init(handler->loop_, &handler->stream_);
        if (rc != 0)
            return;

        handler->stream_.data = handler;
        handler->connect_req_.data = handler;
        rc = uv_tcp_connect(&handler->connect_req_,
                &handler->stream_,
                ai->ai_addr,
                connect_cb);
        if ( rc == 0)
            return;
    }

    handler->Detach();
}

void TcpHandler::connect_cb(uv_connect_t* req, int status)
{
    TcpHandler *handler = (TcpHandler*)req->data;
    if (handler->adapter_){
        if (status == 0){
            handler->adapter_->OnConnect();
        }else{
            handler->adapter_->OnClose();
        }
    }
}
TcpHandler::TcpHandler(uv_loop_t *loop, IOAdapter *adpt):loop_(loop), adapter_(adpt)
{
    rxbuf_.Initialize(4096, 128);
}
TcpHandler::~TcpHandler()
{
    rxbuf_.Destroy();
}

int TcpHandler::Connect(const char *host, uint16_t port)
{
    int rc = 0;
    struct addrinfo hints;
    char portstr[16]={0};

    snprintf(portstr, 16, "%d", port);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    rc= uv_getaddrinfo(loop_,
                &addr_req_,
                conn_getaddrinfo_done,
                host,
                portstr,
                &hints);
    return rc;
}

int TcpHandler::Accept(uv_stream_t *server)
{
    int rc = 0;
    
    rc = uv_tcp_init(loop_, &stream_);
    if (rc != 0)
        return rc;

    rc = uv_accept(server, (uv_stream_t*)&stream_);
    if (rc != 0)
        return rc;

    stream_.data = this;
    // Register read event
    rc = uv_read_start((uv_stream_t*)&stream_, alloc_cb, read_cb);
    if (rc != 0){
        return rc;
    }

    return 0;
}

int TcpHandler::Close()
{
    uv_close((uv_handle_t*)&stream_, NULL);
    return 0;
}
void TcpHandler::Detach()
{
    uv_read_stop((uv_stream_t*)&stream_);
    if (adapter_){
        adapter_->OnClose();
    }
}

