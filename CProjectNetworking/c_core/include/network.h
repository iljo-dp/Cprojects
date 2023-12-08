#ifndef NETWORK_H
#define NETWORK_H

#include <pthread.h>
extern pthread_mutex_t mutex; // Declare as extern
void *clientHandler(void *arg);
void *startServer(void *arg);
void sendData(int clientSocket, const char *data, int dataLength);
void receiveData(int clientSocket, char *buffer, int *dataLength);

int initializeServer();
int acceptConnection(int serverSocket);

#endif /* NETWORK_H */
