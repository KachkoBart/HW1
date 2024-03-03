#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

void set_addr_client_UDP(struct sockaddr_in *servAddr) {
    servAddr->sin_family = AF_INET;
    servAddr->sin_port = htons(3425);
    servAddr->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}
void set_addr_client_TCP(struct sockaddr_in *servAddr) {
    servAddr->sin_family = AF_INET;
    servAddr->sin_port = htons(9001);
    servAddr->sin_addr.s_addr = INADDR_ANY;
}

void set_addr_UDP(struct sockaddr_in *servAddr) {
    servAddr->sin_family = AF_INET;
    servAddr->sin_port = htons(3425);
    servAddr->sin_addr.s_addr = INADDR_ANY;
}
void set_addr_TCP(struct sockaddr_in *servAddr) {
    servAddr->sin_family = AF_INET;
    servAddr->sin_port = htons(9001);
    servAddr->sin_addr.s_addr = INADDR_ANY;
}

int connect_server_TCP() {
    printf("Start client using TCP...\n");
    int sockD = socket(AF_INET, SOCK_STREAM, 0);
    if (sockD < 0) {
        perror("socket does not connect");
        exit(1);
    }

    struct sockaddr_in servAddr;
    set_addr_client_TCP(&servAddr);

    int connectStatus = connect(sockD, (struct sockaddr *) &servAddr, sizeof(servAddr));

    if (connectStatus == -1) {
        perror("Error connection\n");
        exit(1);
    }
    printf("Client connect and ready to send\n");
    return sockD;
}

int connect_server_UDP() {
    printf("Start client using UDP...\n");
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket does not connect");
        return (1);
    }
    struct sockaddr_in addr;
    set_addr_client_UDP(&addr);

    int connectStatus = connect(sock, (struct sockaddr *) &addr, sizeof(addr));
    if (connectStatus == -1) {
        perror("Error connection\n");
        exit(1);
    }
    printf("Client connect and ready to send\n");
    return sock;
}

int start_server_TCP() {
    printf("Start server using TCP...\n");

    int servSockD = socket(AF_INET, SOCK_STREAM, 0);
    if (servSockD < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in servAddr;
    set_addr_TCP(&servAddr);

    if (bind(servSockD, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
        perror("bind");
        exit(2);
    }
    printf("Server bind and ready to work\n");
    return servSockD;
}

int start_server_UDP() {
    printf("Start server using UDP...\n");
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr;
    set_addr_UDP(&addr);

    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(2);
    }
    printf("Server bind and ready to work\n");
    return sock;
}


int main() {
    int num;
    int sock;
    int flag = 0;
    printf("choose: \nTCP server(print '0')\nUDP server(print '1')\nTCP client(print '2')\nUDP client(print '3')\n");
    scanf("%d", &flag);
    if (!flag) {
        sock = start_server_TCP();

        listen(sock, 1);

        int clientSocket = accept(sock, NULL, NULL);
        recv(clientSocket, &num, sizeof(num), 0);
        num *= num;
        send(clientSocket, &num, sizeof(num), 0);
        printf("%d\n", num);
    } else if (flag == 1) {
        sock = start_server_UDP();
        struct sockaddr_in client_addr;
        socklen_t client_addrlen = sizeof(client_addr);

        recvfrom(sock, &num, sizeof(num), 0, (struct sockaddr *) &client_addr, &client_addrlen);
        num *= num;
        sendto(sock, &num, sizeof(num), 0, (struct sockaddr *) &client_addr, client_addrlen);
        printf("%d\n", num);
    } else {
        if (flag == 2) {
            sock = connect_server_TCP();
        } else {
            sock = connect_server_UDP();
        }
        int ans;
        printf("Enter your number:\n");
        scanf("%d", &num);
        send(sock, &num, sizeof(num), 0);

        if (recv(sock, &ans, sizeof(ans), 0) == -1) {
            printf("Server didnt send answer\n");
        } else {
            printf("%d\n", ans);
        }
    }
    shutdown(sock, SHUT_RDWR);
    return 0;
}