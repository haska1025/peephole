#include <string.h>
#include <stdlib.h>
#include "tcplistener.h"
#include "tcphandler.h"
#include "ph_log.h"
#include "ioadapter.h"

void TcpListener::on_connection(uv_stream_t *server, int status)
{
    LOG_INFO("Accept new connection. status(%d)", status);

    if (status == 0){
        TcpListener *tl = (TcpListener*)server->data;
        IOAdapter *adpt = NULL;
        if (!tl->adapter_){
            LOG_ERROR("No tcp listener adapter");
            return;
        }

        adpt = tl->adapter_->OnAccept();
        if (!adpt){
            LOG_ERROR("Create adapter for tcp handler failed");
            return;
        }

        TcpHandler *th = new TcpHandler(tl->loop_, adpt);
        if (!th){
            LOG_ERROR("Create tcphandler failed");
            delete adpt;
            adpt = NULL;
            return;
        }

        th->Accept(server);
    }
}

TcpListener::TcpListener(uv_loop_t *loop, IOAdapter *adpt):IOHandler(loop, adpt)
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
    char *portstr_ptr = NULL;
    char addrbuf[64];
    uint32_t ipv4_naddrs;
    uint32_t ipv6_naddrs;

    memset(&hints, 0, sizeof(struct addrinfo));
    if (port != 0 ){
        snprintf(portstr, 16, "%d", port);
        portstr_ptr = portstr;
    }
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
            portstr_ptr,
            &hints);
    if (rc != 0){
        LOG_ERROR("Tcp listener open failed rc(%d0 errmsg(%s)", rc, uv_strerror(rc));
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
    accept_ = (uv_tcp_t*)malloc(accept_size_ * sizeof(uv_tcp_t));

    int n = 0;
    for (res = req.addrinfo; res != NULL; res = res->ai_next) {
        LOG_INFO("addr info family(%d) socktype(%d)", res->ai_family, res->ai_socktype);
        if (res->ai_family == AF_INET){
            uv_inet_ntop(res->ai_family, (void*)&((struct sockaddr_in*)res->ai_addr)->sin_addr, addrbuf, 63);
            port = ntohs(((struct sockaddr_in*)res->ai_addr)->sin_port);
        }else if ( res->ai_family == AF_INET6) {
            uv_inet_ntop(res->ai_family, (void*)&((struct sockaddr_in6*)res->ai_addr)->sin6_addr, addrbuf, 63);
            port = ntohs(((struct sockaddr_in6*)res->ai_addr)->sin6_port);
        }else{
            continue;
        }

        rc = uv_tcp_init(loop_, &accept_[n]);
        if (rc != 0){
            LOG_WARNING("Init tcp failed rc(%d) rsmsg(%s) addr(%s)", rc, uv_strerror(rc), addrbuf);
            continue;
        }

        rc = uv_tcp_bind(&accept_[n], res->ai_addr, res->ai_family==AF_INET6?UV_TCP_IPV6ONLY:0);
        if (rc != 0){
            LOG_WARNING("Bind tcp failed rc(%d) rsmsg(%s) addr(%s)", rc, uv_strerror(rc), addrbuf);
            uv_close((uv_handle_t*)&accept_[n], NULL);
            continue;
        }

        accept_[n].data = this;
        rc = uv_listen((uv_stream_t *)&accept_[n], 128, on_connection);
        if (rc != 0) {
            LOG_WARNING("Listen tcp failed rc(%d) rsmsg(%s) addr(%s)", rc, uv_strerror(rc), addrbuf);
            uv_close((uv_handle_t*)&accept_[n], NULL);
            continue;
        }

        LOG_INFO("Listening on %s:%u n(%d) family(%d)", addrbuf, port, n, res->ai_family);
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
        uv_close((uv_handle_t*)&accept_[i], NULL);
    }

    free(accept_);
    accept_ = NULL;
    return 0;
}

