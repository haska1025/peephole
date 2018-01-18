#include "tcphandler.h"

TcpHandler::TcpHandler(uv_loop_t *loop):loop_(loop)
{
}
TcpHandler::~TcpHandler()
{
}
int TcpHandler::Accept(uv_stream_t *server)
{
     (0 == uv_tcp_init(sx->loop, &cx->incoming.handle.tcp));
       CHECK(0 == uv_accept(server, &cx->incoming.handle.stream));
}


