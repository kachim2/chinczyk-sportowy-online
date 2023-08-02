
#ifndef NETS
#define NETS
#include <cstdint>
#include <inttypes.h>

struct srvpack{
    uint8_t CurrPawnNum;
    uint8_t CurrPawnMove;
    uint8_t DiceRoll=0;
    uint8_t NextPlayerNum;
    uint8_t WhoAreYou;
};
struct clipack{
    uint8_t PlayerNum;
    uint8_t PawnNum;
    uint16_t GameNum;
};
struct packeddata{
    char data[2] = {0,0};
};
packeddata packcli(clipack packet){
    char data[2] = {0,0};
    data[0] = (packet.PlayerNum << 6);
    data[0] |= (packet.PawnNum << 4) & 0b00110000;
    data[0] |= ((uint8_t)(packet.GameNum >> 8)) & 0b00001111;
    data[1] = (uint8_t)packet.GameNum;
    packeddata d;
    for (int i = 0; i < 2; i++){
        d.data[i] = data[i];
    }
    return d;
}
packeddata packsrv(srvpack packet){
   char data[2] = {0,0};
   data[0] = (packet.CurrPawnNum << 6) & 0b11000000;
   data[0] |= (packet.CurrPawnMove << 3) & 0b00111000;
   data[0] |= (packet.DiceRoll) & 0b00000111;
   data[1] = (packet.NextPlayerNum << 6);
   data[1] |= (packet.WhoAreYou << 4) & 0b00110000;
   packeddata d;
    for (int i = 0; i < 2; i++){
        d.data[i] = data[i];
    }
    return d;
}
clipack unpackcli(packeddata pdata ){
    const char(&data)[2] = pdata.data;
    clipack packet;
    packet.PlayerNum = (data[0] >> 6);
    packet.PawnNum = (data[0] >> 4) & 0b00000011;
    packet.GameNum = 0;
    packet.GameNum |= ((uint16_t)data[0] << 8) & 0b0000111100000000;
    packet.GameNum |= data[1];
    return packet;
}
srvpack unpacksrv(packeddata pdata){
    srvpack packet;
    const char* data = pdata.data;
    packet.CurrPawnNum= (data[0]>>6);
    packet.CurrPawnMove= (data[0] & 0b00111000) >> 3;
    packet.DiceRoll=data[0]&0b00000111;
    packet.NextPlayerNum = (data[1] >> 6) & 0b00000011;
    packet.WhoAreYou= (data[1]  >> 4) & 0b00000011;
    return packet;
}
#endif
