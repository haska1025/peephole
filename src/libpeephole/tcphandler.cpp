#include <string.h>
#include <stdlib.h>
#include "tcphandler.h"
#include "ioadapter.h"
#include "ph_log.h"

void TcpHandler::alloc_cb(uv_handle_t* handle, size_t size, uv_buf_t* buf)
{
    int rc = 0;
    TcpHandler *th = (TcpHandler*)handle->data;

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
    TcpHandler *th = (TcpHandler*)stream->data;
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
void TcpHandler::write_cb(uv_write_t* req, int status)
{
    TcpHandler *handler = (TcpHandler*)req->data;
    if (status == 0){
        handler->ResetUnsentBuf();
        if (handler->adapter_){
            handler->adapter_->OnSend();
        }
    }else{
        handler->Detach();
    }
}
void TcpHandler::conn_getaddrinfo_done(uv_getaddrinfo_t *req, int status, struct addrinfo *ai)
{
    TcpHandler *handler = (TcpHandler*)req->data;

    if (status == 0){
        int rc = 0;
        rc = uv_tcp_init(handler->loop_, &handler->stream_);
        if (rc != 0){
            LOG_ERROR("tcphander(%p) Init tcp failed rc(%d)", handler, rc);
            return;
        }

        handler->stream_.data = handler;
        handler->connect_req_.data = handler;
        rc = uv_tcp_connect(&handler->connect_req_,
                &handler->stream_,
                ai->ai_addr,
                connect_cb);
        if (rc == 0){
            LOG_ERROR("tcphandler(%p) invoke uv tcp connect successfully. rc(%d)" , handler, rc);
            return;
        }
    }

    LOG_INFO("tcphandler(%p) getaddrinfo done status(%d)", handler, status);

    handler->Detach();
}

void TcpHandler::connect_cb(uv_connect_t* req, int status)
{
    TcpHandler *handler = (TcpHandler*)req->data;
    LOG_INFO("tcphandler(%p) connect callback done status(%d)", handler, status);
    if (handler->adapter_){
        if (status == 0){
            // Register read event
            int rc = 0;
            if ((rc = uv_read_start((uv_stream_t*)&handler->stream_, alloc_cb, read_cb)) != 0){
                LOG_ERROR("tcphander(%p) register read event failed. rc (%d)", handler, rc);
                return;
            }

            handler->adapter_->OnConnect();
        }else{
            handler->adapter_->OnClose();
        }
    }
}
TcpHandler::TcpHandler(uv_loop_t *loop, IOAdapter *adpt):IOHandler(loop,adpt)
{
    rxbuf_.Initialize(4096, 128);
    unsent_buf_.base = NULL;
    unsent_buf_.len = 0;
    send_req_.data = this;
    if (adapter_){
        adapter_->iohandler(this);
    }
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
    addr_req_.data = this;

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
int TcpHandler::Send(const void *buff, uint32_t len)
{
    int rc = 0;

    uv_buf_t buf;
    buf.base = (char*)buff;
    buf.len = len;
    rc = uv_try_write((uv_stream_t*)&stream_, &buf, 1);
    LOG_INFO("TCP handler(%p) send size(%u) sentsize(%d)", this, len, rc);
    if (rc < 0){
        if (rc != UV_EAGAIN)
            return rc;
    }
    if (rc < len){
        //Save unsent buffer
        SaveUnsentData(&buf, rc);
        return WantWrite();
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
void TcpHandler::SaveUnsentData(uv_buf_t *sentbuf, uint32_t sentsize)
{
    if (unsent_buf_.base){
        free(unsent_buf_.base);
        unsent_buf_.base = NULL;
    }
    unsent_buf_.len = sentbuf->len - sentsize;
    unsent_buf_.base = (char*)malloc(unsent_buf_.len);
    memcpy(unsent_buf_.base, sentbuf->base+sentsize, unsent_buf_.len);
}
int TcpHandler::WantWrite()
{
    int rc = 0;
    rc = uv_write(&send_req_, (uv_stream_t*)&stream_, &unsent_buf_, 1, write_cb);
    if (rc == 0)
        return -104;

    return rc;
}

void TcpHandler::ResetUnsentBuf()
{
    free(unsent_buf_.base);
    unsent_buf_.base = NULL;
    unsent_buf_.len = 0;
}

