#ifndef _LIBPEEPHOLE_EVENTLOOP_H_
#define _LIBPEEPHOLE_EVENTLOOP_H_

#include "ph_decl.h"
#include <uv.h>

PH_NAMESPACE_DECL_BEGIN

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();
    
    int Start();
    int Stop();
private:
    uv_loop_t *loop_;
};
PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_EVENTLOOP_H_

