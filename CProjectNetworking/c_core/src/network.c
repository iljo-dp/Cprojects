#include "../include/network.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 10240
#define MAX_THREADS 10
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Define mutex here
int initializeServer() {
    int serverSocket;
    struct sockaddr_in serverAddr;

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    return serverSocket;
}

int acceptConnection(int serverSocket) {
    struct sockaddr_in clientAddr;
    int clientSocket;
    socklen_t addrLen = sizeof(clientAddr);

    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);

    if (clientSocket < 0) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    return clientSocket;
}

void sendData(int clientSocket, const char *data, int dataLength) {
    if (send(clientSocket, &dataLength, sizeof(int), 0) == -1) {
        perror("Send data length failed");
        exit(EXIT_FAILURE);
    }

    if (send(clientSocket, data, dataLength, 0) == -1) {
        perror("Send data failed");
        exit(EXIT_FAILURE);
    }
}

void receiveData(int clientSocket, char *buffer, int *receivedDataLength) {
    int bytesRead;
    int dataLength;

    if ((bytesRead = recv(clientSocket, &dataLength, sizeof(int), 0)) == -1) {
        perror("Receive data length failed");
        exit(EXIT_FAILURE);
    }

    *receivedDataLength = dataLength;

    if ((bytesRead = recv(clientSocket, buffer, *receivedDataLength, 0)) == -1) {
        perror("Receive data failed");
        exit(EXIT_FAILURE);
    }

    buffer[*receivedDataLength] = '\0';
    printf("Received data: %s\n", buffer);
}

void *clientHandler(void *arg) {
    int clientSocket = *(int *)arg;
    char buffer[BUFFER_SIZE];

    int dataLength;
    receiveData(clientSocket, buffer, &dataLength);

    printf("Received data from client: %s\n", buffer);

    const char *response = "Message received";
    sendData(clientSocket, response, strlen(response));

    close(clientSocket);
    pthread_exit(NULL);
}

void *startServer(void *arg) {
    int serverSocket = *((int *)arg);
    int clientSocket;
    pthread_t threads[MAX_THREADS];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    while (1) {
        pthread_mutex_lock(&mutex);
        clientSocket = acceptConnection(serverSocket);
        pthread_mutex_unlock(&mutex);

        int i = 0;
        while (i < MAX_THREADS && threads[i] != 0) {
            i++;
        }

        if (i == MAX_THREADS) {
            fprintf(stderr, "Maximum number of threads reached\n");
            close(clientSocket);
        } else {
            if (pthread_create(&threads[i], &attr, clientHandler, &clientSocket) != 0) {
                perror("Thread creation failed");
                exit(EXIT_FAILURE);
            }
        }
    }

    pthread_attr_destroy(&attr);
    close(serverSocket);
    pthread_exit(NULL);
}
