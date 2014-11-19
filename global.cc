#include "global.h"

DECLARE_SINGLETON_MEMBER(Global);

Global::Global(void)
{
    g_reactor_ptr = new reactor::Reactor();
}

Global::~Global(void)
{
    delete g_reactor_ptr;
    g_reactor_ptr = NULL;
}
