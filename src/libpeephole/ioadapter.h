#ifndef _LIBPEEPHOLE_IOADAPTER_H_
#define _LIBPEEPHOLE_IOADAPTER_H_

#include "ph_decl.h"

PH_NAMESPACE_DECL_BEGIN
class IOBuffer;
class IOAdapter
{
public:
    IOAdapter(){}
    virtual ~IOAdapter(){}

    virtual void OnConnect(){}
    virtual int OnRecv(IOBuffer *buf){return 0;}
    virtual int OnSend(){return 0;}
    virtual void OnClose(){}
};
PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_IOADAPTER_H_

