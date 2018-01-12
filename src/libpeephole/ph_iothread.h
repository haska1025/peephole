#ifndef _LIBPEEPHOLE_IOTHREAD_H_
#define _LIBPEEPHOLE_IOTHREAD_H_

#include "ph_decl.h"
#include <uv.h>

PH_NAMESPACE_DECL_BEGIN

class IOThread
{
public:
    IOThread();
    ~IOThread();

    static void thr_svc(void* arg);

    int Start();
    int Stop();
    void WaitStop();

    uv_loop_t *uv_loop(){return loop_;}
private:
    uv_loop_t loop_;
    uv_thread_t worker_;
};
PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_IOTHREAD_H_

