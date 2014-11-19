#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "reactor.h"
#include "singleton.h"

class reactor::Reactor;

class Global : public Singleton<Global>
{
public:
    Global(void);
    ~Global(void);

    reactor::Reactor* g_reactor_ptr;
};

#define sGlobal Global::instance()

#endif
