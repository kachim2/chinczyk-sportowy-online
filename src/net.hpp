
#ifndef NET
#define NET
#include <mutex>
#include <atomic>
#include "shared_net.h"
struct netdata{
    std::atomic_uint8_t Movement;
    std::atomic_uint8_t MovePlayerId;
    std::atomic_uint8_t MyPlayerId;
    std::mutex selecting;
    std::atomic_bool done_main;
};
void net(netdata data);

#endif
