#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define DESIRED_ADDRESS "127.0.0.1"
#define DESIRED_PORT 21376
#define BUFSIZE 2
int main(){
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(DESIRED_PORT); /*converts short to
                                           short with network byte order*/
    addr.sin_addr.s_addr = inet_addr(DESIRED_ADDRESS);
    int sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    listen(sock, 4);
    socklen_t socklen = sizeof addr;
    std::vector<int>clients;
    for (int i = 0; i < 4; i++)
        clients.push_back(accept(sock, (struct sockaddr *)&addr, &socklen));
    for (int i = 0; i < clients.size(); i++){
        send(clients[i], (char *)"He", 2, 0);
        char buf[BUFSIZE];
        recv(clients[i], buf, BUFSIZE, 0);
    }
    close(sock);

    return EXIT_SUCCESS;
}