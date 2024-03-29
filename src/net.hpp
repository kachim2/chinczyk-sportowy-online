
#ifndef NET
#define NET

#include <mutex>
#include <atomic>
#include <SFML/Network.hpp>
struct netdata{
    uint8_t Movement = 0;
    uint8_t MovePlayerId = 0;
    uint8_t MovePawnId = 0;
    uint8_t MyPlayerId = 1;
    uint8_t DiceRoll = 0;
    uint8_t Selected = 0;
    uint8_t GameNum = 0;
    bool selecting = 0;
    bool done_main = 1;

    int next = 0;

    sf::SocketSelector selector;
    sf::TcpSocket socket;

};
void netf(netdata * data);
void net_init(netdata * data);
void net_send(netdata * data);
void net_rec(netdata * data);
bool net_ready(netdata * data);
void net_quit(netdata * data);
#endif
