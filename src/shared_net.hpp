#include <inttypes.h>
struct srvpack{
    uint8_t CurrPawnNum;
    uint8_t CurrPawnMove;
    uint8_t DiceRoll;
    uint8_t NextPlayerNum;
};
struct clipack{
    uint8_t PlayerNum;
    uint8_t PawnNum;
    uint16_t GameNum;
};