#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024

typedef struct {
    uint32_t data;
    char name[30];
    float temperature;
} myData;


int main()
{
    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUF_SIZE];
    int client_addr_len, num_bytes;
    DWORD start_time, end_time;
    
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        exit(EXIT_FAILURE);
    }
    
    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.4.1"); // replace with the server's IP address
    server_addr.sin_port = htons(1234); // replace with the port that the server is listening on
    
    printf("started...\n");

    myData m1 = {0};

    //send a dummy data to notify esp32 UDP server 
    num_bytes = sendto(sockfd, (char*)"Hello I am intruder !", 22, 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    if (num_bytes < 0) {
    perror("sendto failed");
    exit(EXIT_FAILURE);
    }

    do
    {

        clock_t start_time = clock();

        // Receive data from the server with a timestamp
        client_addr_len = sizeof(client_addr);
        num_bytes = recvfrom(sockfd, (char*)&m1, sizeof(myData), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (num_bytes < 0) {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }
        buffer[num_bytes] = '\0';

        clock_t end_time = clock();

        printf("Temp = %f, data = %lu, name = %s\n", m1.temperature, m1.data, m1.name);
        
        // Calculate and print the time taken to send and receive the data
        // printf("Time taken: %d milliseconds\n", end_time - start_time);

        // usleep(1E3);
    }
    while(1);
    

    // Close the socket
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
