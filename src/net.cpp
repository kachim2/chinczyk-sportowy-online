#include "net.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <sys/ioctl.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/websocket.h>
#include <emscripten/threading.h>
#include <emscripten/posix_socket.h>

static EMSCRIPTEN_WEBSOCKET_T bridgeSocket = 0;
#endif
#include "shared_net.h"
#define SERVER_IP "127.0.0.1"
#define PORTC "21376"
#define PORT 21376
void net_init(netdata* data){
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT); /*converts short to
                                       short with network byte order*/
    addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    //freeaddrinfo(result);
    data->sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(data->sock, (struct sockaddr*)&addr, sizeof(addr));
    data->next = 0;
    data->MovePlayerId = 0;
    fcntl(data->sock, F_SETFL, fcntl(data->sock, F_GETFL, 0) | O_NONBLOCK);
}

bool net_ready(netdata* data){
    char tr[2];
    std::cerr << recv(data->sock, tr, 2, MSG_PEEK | MSG_TRUNC);
    return recv(data->sock, tr, 2, MSG_PEEK | MSG_TRUNC) >=2;
}
void net_rec(netdata* data){
    data->MovePlayerId = data->next;
    packeddata srvbuf;
    //fcntl(data->sock, F_SETFL, fcntl(data->sock, F_GETFL, 0) | O_NONBLOCK;
    recv(data->sock, srvbuf.data, 2, 0);
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
    send(data->sock, pdata.data, 2, 0);
}
    /*
void netf(netdata* data)
{

    //struct addrinfo *result;

    //getaddrinfo(SERVER_IP, PORTC, NULL, &result);
    


    while (true)
    {

        
        while(!data->done_main || data->selecting == 1);
        if(spacket.NextPlayerNum == spacket.WhoAreYou)
        {

        }

    }
}
*/