#ifndef _LIBPEEPHOLE_IOHANDLER_H_
#define _LIBPEEPHOLE_IOHANDLER_H_

#include <uv.h>
#include "ph_decl.h"

PH_NAMESPACE_DECL_BEGIN
class IOAdapter;
class IOHandler
{
public:
    IOHandler(uv_loop_t *loop, IOAdapter *adpt);
    virtual ~IOHandler();

    virtual int Connect(const char *host, uint16_t port){return 0;}
    virtual int Send(const void *buff, uint32_t len){return 0;}
    virtual int Accept(uv_stream_t *server){return 0;}
    virtual int Close(){return 0;}
protected:
    uv_loop_t *loop_;
    uv_tcp_t stream_;
    IOAdapter *adapter_;
};
PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_IOHANDLER_H_

