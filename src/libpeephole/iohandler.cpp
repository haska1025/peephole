#include "iohandler.h"


IOHandler::IOHandler(uv_loop_t *loop, IOAdapter *adpt):loop_(loop), adapter_(adpt)
{
}

IOHandler::~IOHandler()
{
}

