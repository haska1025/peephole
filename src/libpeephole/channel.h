#ifndef _LIBPEEPHOLE_CHANNEL_H_
#define _LIBPEEPHOLE_CHANNEL_H_

#include "ph_decl.h"

PH_NAMESPACE_DECL_BEGIN

class Channel
{
public:
    Channel(uint32_t id);
    ~Channel();

    uint32_t cid(){return id_;}
private:
    uint32_t id_;
};
PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_CHANNEL_H_

