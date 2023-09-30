#include "net.hpp"

#ifndef USE_SFML
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <sys/ioctl.h>
//#ifdef __EMSCRIPTEN__
//#include <emscripten.h>
/*#include <emscripten/websocket.h>
#include <emscripten/threading.h>
#include <emscripten/posix_socket.h>

static EMSCRIPTEN_WEBSOCKET_T bridgeSocket = 0;
#endif*/

#include "shared_net.h"
#define SERVER_IP "127.0.0.1"
#define PORTC "21376"
#define PORT 21376
void net_init(netdata* data){
/*#ifdef __EMSCRIPTEN__
    bridgeSocket = emscripten_init_websocket_to_posix_socket_bridge("ws://localhost:8050");
  // Synchronously wait until connection has been established.
  uint16_t readyState = 0;
  do {
    emscripten_websocket_get_ready_state(bridgeSocket, &readyState);
    emscripten_thread_sleep(100);
  } while (readyState == 0);
#endif*/


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
#else


#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <sys/ioctl.h>
//#ifdef __EMSCRIPTEN__
//#include <emscripten.h>
/*#include <emscripten/websocket.h>
#include <emscripten/threading.h>
#include <emscripten/posix_socket.h>

static EMSCRIPTEN_WEBSOCKET_T bridgeSocket = 0;
#endif*/
#include "shared_net.h"
#define SERVER_IP "127.0.0.1"
#define PORTC "21376"
#define PORT 21376
void net_init(netdata* data){
/*#ifdef __EMSCRIPTEN__
    bridgeSocket = emscripten_init_websocket_to_posix_socket_bridge("ws://localhost:8050");
  // Synchronously wait until connection has been established.
  uint16_t readyState = 0;
  do {
    emscripten_websocket_get_ready_state(bridgeSocket, &readyState);
    emscripten_thread_sleep(100);
  } while (readyState == 0);
#endif*/


    data->socket;
    data->socket.connect(SERVER_IP, PORT);
    data->selector.add(data->socket);
    data->next = 0;
    data->MovePlayerId = 0;
    data->socket.setBlocking(false);
}

bool net_ready(netdata* data){

    char d;
    size_t r;
    return (data->socket.receive(&d, 0, r) == sf::Socket::Done);
}
void net_rec(netdata* data){
    data->MovePlayerId = data->next;
    packeddata srvbuf;
    //fcntl(data->sock, F_SETFL, fcntl(data->sock, F_GETFL, 0) | O_NONBLOCK;
    std::size_t recieved;
    data->socket.setBlocking(true);
    data->socket.receive(srvbuf.data, 2, recieved);
    data->socket.setBlocking(false);
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
#endif