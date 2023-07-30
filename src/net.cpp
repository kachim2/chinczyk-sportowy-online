#include "net.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "shared_net.h"
#define SERVER_IP "127.0.0.1"
#define PORT 21376

void net(netdata &data)
{
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    int sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    data.MovePlayerId = 0;
    while (true){
        data.done_main = 1;
        packeddata srvbuf;
        recv(sock, srvbuf.data, 2, 0);
        srvpack spacket = unpacksrv(srvbuf);
        data.MovePlayerId = spacket.WhoAreYou;
        data.MovePawnId = spacket.CurrPawnNum;
        data.Movement = spacket.CurrPawnMove;
        if(spacket.NextPlayerNum==spacket.WhoAreYou){
            data.selecting = 1;
        }
        data.done_main = 0;
        while(!data.done_main);
        if(spacket.NextPlayerNum == spacket.WhoAreYou)
        {
            clipack cpacket;
            cpacket.GameNum = data.GameNum;
            cpacket.PawnNum = data.Selected;
            cpacket.PlayerNum = spacket.WhoAreYou;
        }
        data.MovePlayerId = spacket.NextPlayerNum;
    }
}
