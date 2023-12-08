#include "../c_core/include/network.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 10240
#define MAX_THREADS 10
#define AES_BLOCK_SIZE 16

const unsigned char encryptionKey[AES_BLOCK_SIZE] = "example";
const unsigned char iv[AES_BLOCK_SIZE] = "example";
int main() {
  int serverSocket = initializeServer();

  printf("Server started. Waiting for incoming connections...\n");

  while (1) {
    int clientSocket = acceptConnection(serverSocket);

    printf("Client connected. Creating a new thread for the client...\n");

    pthread_t thread;
    if (pthread_create(&thread, NULL, clientHandler, &clientSocket) != 0) {
      perror("Thread creation failed");
      exit(EXIT_FAILURE);
    }

    // Wait for thread completion (optional)
    pthread_join(thread, NULL);
  }

  close(serverSocket);
  return 0;
}
