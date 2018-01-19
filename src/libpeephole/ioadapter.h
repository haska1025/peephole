#ifndef _LIBPEEPHOLE_IOADAPTER_H_
#define _LIBPEEPHOLE_IOADAPTER_H_

#include <uv.h>
#include "ph_decl.h"

PH_NAMESPACE_DECL_BEGIN
class IOAdapter
{
public:
    IOAdapter(){}
    virtual ~IOAdapter(){}

    virtual void OnConnect(){}
    virtual int OnRecv(uv_buf_t *buf){return 0;}
    virtual int OnSend(){return 0;}
    virtual void OnClose(){}
};
PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_IOADAPTER_H_

