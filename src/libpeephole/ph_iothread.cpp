#include "ph_iothread.h"
#include "ph_log.h"


void IOThread::async_cb(uv_async_t* handle)
{
    // Do nothing
    IOThread *iothr = (IOThread*)handle->data;
    iothr->state_ = 0;    
    uv_stop(&iothr->loop_);
}
void IOThread::thr_svc(void* arg)
{
    IOThread *iothr = (IOThread*)arg;

    LOG_INFO("Enter the iothread(%p)", arg);

    if (0 != uv_loop_init(&iothr->loop_)){
        LOG_ERROR("Start worker thread failed");
        return;
    }

    if (0 != uv_async_init(&iothr->loop_, &iothr->notify_, async_cb)){
        LOG_ERROR("Start worker thread. init async failed");
        return;
    }
    iothr->notify_.data = arg;

    iothr->state_ = 1;
    uv_mutex_lock(&iothr->state_mutex_);
    uv_cond_broadcast(&iothr->state_cond_);
    uv_mutex_unlock(&iothr->state_mutex_);

    uv_run(&iothr->loop_, UV_RUN_DEFAULT);
    uv_loop_close(&iothr->loop_);

    LOG_INFO("Leave the iothread(%p)", arg);
}

IOThread::IOThread()
{
    state_ = 0;
    uv_mutex_init(&state_mutex_);
    uv_cond_init(&state_cond_);
}
IOThread::~IOThread()
{
}
int IOThread::Start()
{
    int rc = 0;

    if (state_ == 1)
        return 0;

    uv_mutex_lock(&state_mutex_);
    if (state_ == 1){
        uv_mutex_unlock(&state_mutex_);
        return 0;
    }
    uv_mutex_unlock(&state_mutex_);

    rc = uv_thread_create(&worker_, thr_svc, (void*)this); 
    if (rc != 0){
        LOG_ERROR("Start iothread failed rc(%d)", rc);
        return rc;
    }
    
    uv_mutex_lock(&state_mutex_);
    while( state_ != 1){
        rc = uv_cond_timedwait(&state_cond_, &state_mutex_, 150 * 1e6);
        if (rc != 0){
            break;
        }
    }
    uv_mutex_unlock(&state_mutex_);

    return rc;
}
int IOThread::Stop()
{
    uv_mutex_lock(&state_mutex_);
    state_ = 0;
    uv_mutex_unlock(&state_mutex_);
    uv_async_send(&notify_);
    return 0;
}
void IOThread::WaitStop()
{
    uv_thread_join(&worker_);
}

