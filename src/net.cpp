#include "net.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "shared_net.h"
#define SERVER_IP "130.61.226.238"
#define PORT 21376

void netf(netdata* data)
{
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    int sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
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
