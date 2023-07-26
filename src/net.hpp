
#ifndef NET
#define NET
#include <mutex>
#include <atomic>
struct netdata{
    std::atomic_uint8_t Movement;
    std::atomic_uint8_t MovePlayerId;
    std::atomic_uint8_t MyPlayerId;
    std::mutex selecting;
    std::atomic_bool done_main;
};
void net(netdata)
{
}

#endif