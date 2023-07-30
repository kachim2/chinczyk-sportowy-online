
#ifndef NET
#define NET
#include <mutex>
#include <atomic>

struct netdata{
    std::atomic_uint8_t Movement;
    std::atomic_uint8_t MovePlayerId;
    std::atomic_uint8_t MovePawnId;
    std::atomic_uint8_t MyPlayerId;
    std::atomic_uint8_t DiceRoll;
    std::atomic_uint8_t Selected;
    std::atomic_uint8_t GameNum;
    std::atomic_bool selecting;
    std::atomic_bool done_main;
};
void net(netdata data);

#endif
