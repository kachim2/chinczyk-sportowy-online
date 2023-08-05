#include "net.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <netdb.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/websocket.h>
#include <emscripten/threading.h>
#include <emscripten/posix_socket.h>

static EMSCRIPTEN_WEBSOCKET_T bridgeSocket = 0;
#endif
#include "shared_net.h"
#define SERVER_IP "sq2ips.ddns.net"
#define PORTC "21376"
#define PORT 21376

void netf(netdata* data)
{
    #ifdef __EMSCRIPTEN__
    bridgeSocket = emscripten_init_websocket_to_posix_socket_bridge("ws://sq2ips.ddns.net:21375");
    // Synchronously wait until connection has been established.
    uint16_t readyState = 0;
    do {
    emscripten_websocket_get_ready_state(bridgeSocket, &readyState);
    emscripten_thread_sleep(100);
    } while (readyState == 0);
    #endif
    struct addrinfo *result;

    getaddrinfo(SERVER_IP, PORTC, NULL, &result);
    
    struct sockaddr addr = *result->ai_addr;
    freeaddrinfo(result);
    int sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(sock, &addr, sizeof(addr));
    data->MovePlayerId = 0;
    int next = 0;
    while (true)
    {
        data->done_main = 1;
        packeddata srvbuf;
        recv(sock, srvbuf.data, 2, 0);
        srvpack spacket = unpacksrv(srvbuf);
        data->MovePlayerId = next;
        data->MovePawnId = spacket.CurrPawnNum;
        data->Movement = spacket.CurrPawnMove;
        data->MyPlayerId = spacket.WhoAreYou;
        data->DiceRoll = spacket.DiceRoll;
        next = spacket.NextPlayerNum;
        if (spacket.NextPlayerNum == spacket.WhoAreYou)
        {
            std::cout << "HMM";
            data->selecting = 1;
        }
        data->done_main = 0;
        
        while(!data->done_main || data->selecting == 1);
        if(spacket.NextPlayerNum == spacket.WhoAreYou)
        {
            clipack cpacket;
            cpacket.GameNum = data->GameNum;
            cpacket.PawnNum = data->Selected;
            cpacket.PlayerNum = spacket.WhoAreYou;
            packeddata pdata = packcli(cpacket);
            send(sock, pdata.data, 2, 0);
        }
        data->MovePlayerId = spacket.NextPlayerNum;
    }
}
