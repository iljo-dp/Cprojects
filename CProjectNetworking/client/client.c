#include "../c_core/include/network.h"
#include <arpa/inet.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 10240
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
  int clientSocket;
  struct sockaddr_in serverAddr;
  char buffer[BUFFER_SIZE];

  if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
  serverAddr.sin_port = htons(SERVER_PORT);

  if (connect(clientSocket, (struct sockaddr *)&serverAddr,
              sizeof(serverAddr)) == -1) {
    perror("Connection failed");
    exit(EXIT_FAILURE);
  }

  const char *testData = "test data";
  int testDataLength = strlen(testData); // Calculate the length

  sendData(clientSocket, testData, testDataLength);

  int receivedDataLength; // Define a variable to hold the received data length
  receiveData(clientSocket, buffer, &receivedDataLength);

  printf("Received data from server: %s\n", buffer);

  close(clientSocket);
  return 0;
}
