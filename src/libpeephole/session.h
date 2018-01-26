#ifndef _LIBPEEPHOLE_SESSION_H_
#define _LIBPEEPHOLE_SESSION_H_

#include <unordered_map>
#include "ph_decl.h"
#include "channel.h"

PH_NAMESPACE_DECL_BEGIN
class Session
{
public:
    Session(uint32_t id);
    ~Session();

    uint32_t sid(){return id_;}
private:
    uint32_t id_;
    std::unordered_map<uint32_t, Channel*> channels_;//UserID, Channel
};

PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_SESSION_H_

