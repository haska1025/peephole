#ifndef _LIBPEEPHOLE_USER_H_
#define _LIBPEEPHOLE_USER_H_

#include <unordered_map>
#include "ph_decl.h"
#include "channel.h"

PH_NAMESPACE_DECL_BEGIN
class TcpConnection;
class User
{
public:
    User(uint32_t id);
    ~User();

    uint32_t uid(){return id_;}
private:
    uint32_t id_;
    std::unordered_map<uint32_t, Channel*> channels_;
    TcpConnection *conn_;
};
PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_USER_H_

