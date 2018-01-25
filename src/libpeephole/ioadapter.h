#ifndef _LIBPEEPHOLE_IOADAPTER_H_
#define _LIBPEEPHOLE_IOADAPTER_H_

#include "ph_decl.h"

PH_NAMESPACE_DECL_BEGIN
class IOBuffer;
class IOHandler;
class IOAdapter
{
public:
    IOAdapter():handler_(NULL){}
    virtual ~IOAdapter(){}

    virtual IOAdapter * OnAccept(){return NULL;}
    virtual void OnConnect(){}
    virtual int OnRecv(IOBuffer *buf){return 0;}
    virtual int OnSend(){return 0;}
    virtual void OnClose(){}

    void iohandler(IOHandler *ioh){handler_ = ioh;}
protected:
    IOHandler *handler_;
};
PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_IOADAPTER_H_

