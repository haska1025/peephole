#include "iothread.h"


void IOThread::thr_svc(void* arg)
{
    IOThread *iothr = (IOThread*)arg;
}

IOThread::IOThread()
{
}
IOThread::~IOThread()
{
}
int IOThread::Start()
{
    int rc = 0;

    rc = uv_thread_create(&worker_, thr_svc, (void*)this); 
    if (rc != 0){
        printf("
    }
}
int IOThread::Stop()
{
}
void IOThread::WaitStop()
{
}

