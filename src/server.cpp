#include <arpa/inet.h>
#include <cstddef>
#include <cstdlib>
#include <netinet/in.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "shared_net.h"
#include <iostream>
#include <time.h>
#define DESIRED_ADDRESS "127.0.0.1"
#define DESIRED_PORT 21376
#define BUFSIZE 2
int main()
{
    srand(time(NULL));
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(DESIRED_PORT); /*converts short to
                                           short with network byte order*/

    addr.sin_addr.s_addr = inet_addr(DESIRED_ADDRESS);
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    listen(sock, 4);
    socklen_t socklen = sizeof addr;
    std::vector<int> clients;
    for (int i = 0; i < 4; i++)
        clients.push_back(accept(sock, (struct sockaddr *)&addr, &socklen));
    std::cerr << "got it";
    int pn = 0;
    int sel = 0;
    int move = 0;
    bool inplays[4][4] = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}};
    int pos[4][4] = {
        {0, 0, 0, 0},
        {10, 10, 10, 10},
        {20, 20, 20, 20},
        {30, 30, 30, 30}};
    bool dicerollallowed = 1;
    int diceroll = 0;
    const int poslimit = 43;
    while (true)
    {
        if (dicerollallowed)
            diceroll = (rand() % 6) + 1;
        dicerollallowed = 1;

        for (int i = 0; i < clients.size(); i++)
        {

            srvpack spack;

            spack.CurrPawnMove = move;
            spack.CurrPawnNum = sel;
            spack.WhoAreYou = i;
            spack.NextPlayerNum = pn;
            spack.DiceRoll = diceroll;
            packeddata pdata = packsrv(spack);
            send(clients[i], pdata.data, 2, 0);
        }

        packeddata buf;
        recv(clients[pn], buf.data, 2, 0);
        clipack cpack = unpackcli(buf);
        sel = cpack.PawnNum;
        if (diceroll != 6 &&
            (inplays[pn][sel] == 0) ||
            ((pos[pn][sel] - pn * 10) + diceroll > poslimit))
        {

            move = 0;
            dicerollallowed = 0;
            bool domovesexist = 0;
            for (int i = 0; i < 4; i++)
            {
                if (inplays[pn][i] == 1 && (((pos[pn][sel] - pn * 10) + diceroll <= poslimit)))
                {
                    domovesexist = 1;
                    break;
                }
            }
            if (!domovesexist)
            {
                dicerollallowed = 1;
                pn++;
                pn %= 4;
            }
        }
        else
        {
            inplays[pn][sel] = 1;
            pos[pn][sel] += move;
            move = diceroll;

            pn++;
            pn %= 4;
        }
    }
    close(sock);

    return EXIT_SUCCESS;
}
