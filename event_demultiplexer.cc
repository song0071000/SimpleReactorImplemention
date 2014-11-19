#include <errno.h>
#include <assert.h>
#include <vector>
#include "event_demultiplexer.h"

namespace reactor
{
    EpollDemultiplexer::EpollDemultiplexer()
    {
        m_epoll_fd = ::epoll_create(FD_SETSIZE);
        assert(m_epoll_fd != -1);
        m_fd_num = 0;
    }

    EpollDemultiplexer::~EpollDemultiplexer()
    {
        ::close(m_epoll_fd);
    }

    int EpollDemultiplexer::WaitEvents(
            std::map<handle_t, EventHandler *> * handlers,
            int timeout, time_heap* event_timer)
    {
        std::vector<epoll_event> ep_evts(m_fd_num);
        int num = epoll_wait(m_epoll_fd, &ep_evts[0], ep_evts.size(), timeout);
        if (num > 0)
        {
            for (int idx = 0; idx < num; ++idx)
            {
                handle_t handle = ep_evts[idx].data.fd;
                assert(handlers->find(handle) != handlers->end());
                if ((ep_evts[idx].events & EPOLLERR) ||
                        (ep_evts[idx].events & EPOLLHUP))
                {
                    (*handlers)[handle]->HandleError();
                }
                else
                {
                    if (ep_evts[idx].events & EPOLLIN)
                    {
                        (*handlers)[handle]->HandleRead();
                    }
                    if (ep_evts[idx].events & EPOLLOUT)
                    {
                        (*handlers)[handle]->HandleWrite();
                    }
                }
            }
        }
        if (event_timer != NULL)
        {
            event_timer->tick();
        }

        return num;
    }

    int EpollDemultiplexer::RequestEvent(handle_t handle, event_t evt)
    {
        epoll_event ep_evt;
        ep_evt.data.fd = handle;
        ep_evt.events = 0;

        if (evt & kReadEvent)
        {
            ep_evt.events |= EPOLLIN;
        }
        if (evt & kWriteEvent)
        {
            ep_evt.events |= EPOLLOUT;
        }
        ep_evt.events |= EPOLLONESHOT;

        if (epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, handle, &ep_evt) != 0)
        {
            if (errno == ENOENT)
            {
                if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, handle, &ep_evt) != 0)
                {
                    return -errno;
                }
                ++m_fd_num;
            }
        }
        return 0;
    }

    int EpollDemultiplexer::UnrequestEvent(handle_t handle)
    {
        epoll_event ep_evt;
        if (epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, handle, &ep_evt) != 0)
        {
            return -errno;
        }
        --m_fd_num;
        return 0;
    }
} // namespace reactor

