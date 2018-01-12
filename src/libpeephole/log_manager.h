#ifndef _LOG_MANAGER_H_ 
#define _LOG_MANAGER_H_ 

#include "ph_decl.h"

PH_NAMESPACE_DECL_BEGIN

class LoggerManager
{
public:
    static LoggerManager* Instance();

    void SetLogLevel(int level){log_level_ = level;}
    int GetLogLevel(){return log_level_;}

private:
    static LoggerManager s_inst;

    int log_level_;
private:
    DISALLOW_CONSTRUCT(LoggerManager);
};

PH_NAMESPACE_DECL_END

#endif//_LOG_MANAGER_H_ 

