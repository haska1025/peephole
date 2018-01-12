#ifndef _PH_LOG_H_
#define _PH_LOG_H_

#include <syslog.h>
#include "ph_decl.h"
#include "log_manager.h"

PH_NAMESPACE_DECL_BEGIN

#ifdef LOG_ERROR
#undef LOG_ERROR
#endif

#ifdef LOG_WARNING
#undef LOG_WARNING
#endif

#ifdef LOG_INFO
#undef LOG_INFO
#endif

#ifdef LOG_DEBUG
#undef LOG_DEBUG
#endif
#define __TCM_BASE_LOG(level,fmt, ...) if (level<=LoggerManager::Instance()->GetLogLevel()) syslog(LOG_LOCAL3|level, "%s:%d:(%s): "fmt , __FILE__,__LINE__,__FUNCTION__, ##__VA_ARGS__);

#define LOG_ERROR(fmt, ...)  __TCM_BASE_LOG(3, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...)  __TCM_BASE_LOG(4, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  __TCM_BASE_LOG(6, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)  __TCM_BASE_LOG(7, fmt, ##__VA_ARGS__)

PH_NAMESPACE_DECL_END
#endif//_PH_LOG_H_


