#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define QUEUE_SIZE 5
#define PORT 8080

int main(int argc, const char *argv[]) {
    int server_socket, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening 0.0.0.0:%d \n", PORT);
    if (listen(server_socket, QUEUE_SIZE) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((client_socket = accept(server_socket, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("Client connected \n");

    long byteRead;
    do {
        char buffer[BUFFER_SIZE];
        byteRead = read(client_socket, buffer, BUFFER_SIZE);

        if (byteRead <= 0) {
            printf("Connection has already been closed \n");
            break;
        }

        buffer[byteRead] = '\0';
        printf("Message: %s", buffer);
    } while (1);

    close(server_socket);
    close(client_socket);
    return 0;
}
