#include "log_manager.h"

LoggerManager LoggerManager::s_inst;

LoggerManager::LoggerManager()
{
    log_level_ = 6;
}

LoggerManager* LoggerManager::Instance()
{
    return &s_inst;
}

