#include "tcplistener.h"

void TcpListener::on_connection(uv_stream_t *server, int status)
{
    server_ctx *sx;
    client_ctx *cx;

    CHECK(status == 0);
    sx = CONTAINER_OF(server, server_ctx, tcp_handle);
    cx = xmalloc(sizeof(*cx));
    CHECK(0 == uv_tcp_init(sx->loop, &cx->incoming.handle.tcp));
    CHECK(0 == uv_accept(server, &cx->incoming.handle.stream));
    client_finish_init(sx, cx);
}

TcpListener::TcpListener(uv_loop_t *loop):loop_(loop)
{
    accept_ = NULL;
    accept_size_ = 0;
}
TcpListener::~TcpListener()
{
}
int TcpListener::Open(const char *host, unsigned int port)
{
    int rc = 0;
    uv_getaddrinfo_t req;
    struct addrinfo hints, *res;
    char portstr[16]={0};
    char addrbuf[64];
    uint32_t ipv4_naddrs;
    uint32_t ipv6_naddrs;

    memset(&hints, 0, sizeof(struct addrinfo));
    snprintf(portstr, 16, "%s", port);
    /*
     * AI_PASSIVE flag: the resulting address is used to bind
     * to a socket for accepting incoming connections.
     * So, when the hostname==NULL, getaddrinfo function will
     * return one entry per allowed protocol family containing
     * the unspecified address for that family.
     */
    res = NULL;
    hints.ai_flags    = AI_PASSIVE;
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    rc = uv_getaddrinfo(loop_,
            &req,
            NULL,
            host,
            portstr,
            NULL);
    if (rc != 0){
        LOG_ERROR("Tcp listener open failed rc(%d0 errmsg(%s)", rc, uv_streror(rc));
        return -1;
    }

    ipv4_naddrs = 0;
    ipv6_naddrs = 0;
    for (res = req.addrinfo; res != NULL; res = res->ai_next) {
        if (res->ai_family == AF_INET) {
            ipv4_naddrs += 1;
        } else if (res->ai_family == AF_INET6) {
            ipv6_naddrs += 1;
        }
    }

    if (ipv4_naddrs == 0 && ipv6_naddrs == 0) {
        LOG_ERROR("%s has no IPv4/6 addresses", host);
        uv_freeaddrinfo(req.addrinfo);
        return -1;
    }

    accept_size_ = ipv4_naddrs + ipv6_naddrs;
    accept_ = malloc(accept_size_ * sizeof(uv_tcp_t));

    int n = 0;
    for (res = req.addrinfo; res != NULL; res = res->ai_next) {
        if (res->ai_family != AF_INET && res->ai_family != AF_INET6) {
            continue;
        }

        uv_inet_ntop(res->ai_family, res->ai_addr, addrbuf, 63);
        rc = uv_tcp_init(loop_, &accpet_[n]);
        if (rc != 0){
            LOG_WARNING("init tcp failed rc(%d) rsmsg(%s) addr(%s)", rc, uv_strerror(rc), addrbuf);
            continue;
        }

        rc = uv_tcp_bind(&accept_[n], res, 0);
        if (rc != 0){
            LOG_WARNING("init tcp failed rc(%d) rsmsg(%s) addr(%s)", rc, uv_strerror(rc), addrbuf);
            uv_close(&accept_[n], NULL);
            continue;
        }

        rc = uv_listen((uv_stream_t *)&accept_[n], 128, on_connection);
        if (rc == 0) {
            LOG_WARNING("init tcp failed rc(%d) rsmsg(%s) addr(%s)", rc, uv_strerror(rc), addrbuf);
            uv_close(&accept_[n], NULL);
            continue;
        }

        LOG_INFO("listening on %s:%hu", addrbuf, cf->bind_port);
        n += 1;
    }
    if (n == 0)
        return -1;

    uv_freeaddrinfo(req.addrinfo);

    return 0;
}
int TcpListener::Close()
{
    for (int i = 0; i < accept_size_; ++i){
        uv_close(&accept_[i], NULL);
    }

    free(accept_);
    accept_ = NULL;
    return 0;
}

