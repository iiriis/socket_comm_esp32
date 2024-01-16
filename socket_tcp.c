#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <signal.h>
#include <time.h>
#include <stdint.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "192.168.4.1" // Replace with the IP address of the ESP32 server
#define SERVER_PORT 80 // Replace with the port number used by the ESP32 server

SOCKET sockfd; // Global socket variable


typedef struct {
    uint64_t data;
    char name[30];
    float temperature;
} myData;

// Signal handler for SIGINT (Ctrl+C)
void sigint_handler(int sig) {
    printf("Exiting...\n");
    closesocket(sockfd);
    WSACleanup();
    exit(0);
}

int main() {
    WSADATA wsaData;
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) {
        printf("WSAStartup failed with error: %d\n", res);
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
        printf("Unable to create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    res = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (res == SOCKET_ERROR) {
        printf("Unable to connect to server: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    int flag = 1;
    int result = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));
    if (result == SOCKET_ERROR) {
        printf("Unable to set TCP_NODELAY option: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    signal(SIGINT, sigint_handler); // Register signal handler for SIGINT (Ctrl+C)


    myData m1 = {0};

    // Send data to the server
    const char* data = "Hello from PC";
    send(sockfd, data, strlen(data), 0);

    while (1) {

        char buffer[1024];

        clock_t start = clock(); // Start the clock


        // Receive response from the server
        int n = recv(sockfd, (char*)&m1, sizeof(myData), 0); 
        if (n > 0) {
            buffer[n] = '\0';
            printf("Temp = %f, data = %lu, name = %s\n", m1.temperature, m1.data, m1.name);
        }

        clock_t end = clock(); // Stop the clock
        double elapsed_time = (double)(end - start); // Calculate elapsed time in mili seconds

    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
