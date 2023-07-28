#include "net.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#define SERVER_IP "127.0.0.1"
#define PORT 21376

void net(netdata data)
{
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    int sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
}
