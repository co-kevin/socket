#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUFFER_SIZE 4096
#define PORT 8080

int main(int argc, const char *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error \n");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed \n");
        exit(EXIT_FAILURE);
    }
    printf("Server connected\n");

    char buffer[BUFFER_SIZE];
    long byteSend;
    do {
        fgets(buffer, BUFFER_SIZE, stdin);
        if (buffer[0] == '\n') {
            printf("Close the connection\n");
            break;
        }

        byteSend = send(sock, buffer, strlen(buffer), 0);
        if (byteSend != strlen(buffer)) {
            printf("Send error\n");
            exit(EXIT_FAILURE);
        }
    } while (1);
    close(sock);
    return 0;
}
