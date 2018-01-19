#include "tcphandler.h"

TcpHandler::TcpHandler(uv_loop_t *loop, IOAdapter *adpt):loop_(loop), adapter_(adpt)
{
}
TcpHandler::~TcpHandler()
{
}
int TcpHandler::Accept(uv_stream_t *server)
{
    int rc = 0;
    
    rc = uv_tcp_init(loop_, &stream_);
    if (rc != 0)
        return rc;

    rc = uv_accept(server, &stream_);
    if (rc != 0)
        return rc;

    // Register read event
    rc = uv_read_start((uv_stream_t*)&stream_, alloc_cb, read_cb);
    if (rc != 0){
        return rc;
    }

    return 0;
}

int TcpHandler::Close()
{
    uv_close(&stream_, NULL);
    return 0;
}

void TcpHandler::alloc_cb(uv_handle_t* handle, size_t size, uv_buf_t* buf)
{
    buf->base = malloc(size);
    buf->len = size;
}
void TcpHandler::read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    if (nread > 0){
        if (adapter_){
            adapter_->OnRecv(buf);
        }
    }else if(nread == 0){
        return;
    }else{
        Detach();
    }
}

void TcpHandler::Detach()
{
    uv_read_stop((uv_stream_t*)&stream_);
    if (adapter_){
        adapter_->OnClose();
    }
}

