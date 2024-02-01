#include "net.hpp"
#include <iostream>
#include <memory>
#include "shared_net.h"
#define SERVER_IP "130.61.62.195"
#define PORTC "21376"
#define PORT 21376
void net_init(netdata* data){



    data->socket.connect(SERVER_IP, PORT);
    data->selector.add(data->socket);
    data->next = 0;
    data->MovePlayerId = 0;
    data->socket.setBlocking(true);
    net_send(data);
}

bool net_ready(netdata* data){

//    char d;
//    size_t r;
//    return (data->socket.receive(&d, 0, r) == (sf::Socket::Done || sf::Socket::Disconnected));
data->selector.wait(sf::microseconds(10));
    return data->selector.isReady(data->socket);
}
void net_quit(netdata* data){
    unsigned char d[2] = {255, 255};
    data->socket.send(data, 2);
}
void net_rec(netdata* data){
    data->MovePlayerId = data->next;
    packeddata srvbuf;
    //fcntl(data->sock, F_SETFL, fcntl(data->sock, F_GETFL, 0) | O_NONBLOCK;
    std::size_t recieved;

    data->socket.receive(srvbuf.data, 2, recieved);

    //fcntl(data->sock, F_SETFL, fcntl(data->sock, F_GETFL, 0) | O_NONBLOCK);
    srvpack spacket = unpacksrv(srvbuf);
    data->MovePlayerId = data->next;
    data->MovePawnId = spacket.CurrPawnNum;
    data->Movement = spacket.CurrPawnMove;
    data->MyPlayerId = spacket.WhoAreYou;
    data->DiceRoll = spacket.DiceRoll;
    data->next = spacket.NextPlayerNum;
    if (spacket.NextPlayerNum == spacket.WhoAreYou)
    {
        std::cout << "HMM";
        data->selecting = 1;
    }
    data->done_main = 0;
}

void net_send(netdata* data) {
    clipack cpacket;
    cpacket.GameNum = data->GameNum;
    cpacket.PawnNum = data->Selected;
    cpacket.PlayerNum = data->MyPlayerId;
    packeddata pdata = packcli(cpacket);
    data->socket.send(pdata.data, 2);
}

