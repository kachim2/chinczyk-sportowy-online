
#ifndef NET
#define NET
#include <mutex>
#include <atomic>

struct netdata{
    std::atomic_uint8_t Movement = 0;
    std::atomic_uint8_t MovePlayerId = 0;
    std::atomic_uint8_t MovePawnId = 0;
    std::atomic_uint8_t MyPlayerId = 1;
    std::atomic_uint8_t DiceRoll = 0;
    std::atomic_uint8_t Selected = 0;
    std::atomic_uint8_t GameNum = 0;
    std::atomic_bool selecting = 0;
    std::atomic_bool done_main = 1;
};
void netf(netdata * data);

#endif
