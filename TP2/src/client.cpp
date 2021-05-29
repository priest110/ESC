#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <string>
#include "hash.h"

#define PORT    6000            // port
#define N       1024            // array size

int main(int argc, char *argv[]){
    int client_id = atoi(argv[1]);
    int sock = 0, val;
    struct sockaddr_in serv_address;             // structure for handling internet addresses
    char buffer[N*N] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
        perror("criação da socket falhou");

    /* Setup do address para o bind */
    serv_address.sin_family = AF_INET;           // Server byte order
    serv_address.sin_port = htons(PORT);         // Convert short integer value for port must be converted into network byte order
    serv_address.sin_addr.s_addr = inet_addr("127.0.0.1");
   
    /* Conecta */
    if(connect(sock, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0)
        perror("Conexão falhou\n");

    srand(time(nullptr));
    while(true){
        std::string option = std::to_string(rand() % 21 == 0 ? 1 : 0)  + "\n";
        bzero(buffer, N*N);
        strcat(buffer, option.c_str());
        if(option.compare("0\n") == 0){
            std::string key = std::to_string(rand() % RAND_MAX);
            strcat(buffer, key.c_str());
        }
        else{
            std::string key = std::to_string(rand() % RAND_MAX) + "\n";
            std::string value = hash::random_string(N);
            strcat(buffer, key.c_str());
            strcat(buffer, value.c_str());
        }
        //printf("Mandei (%d)\n", client_id);
        //auto start = std::chrono::steady_clock::now();
        send(sock, buffer, strlen(buffer), 0);
        bzero(buffer, N*N);
        val = read(sock, buffer, N*N);
        //auto end = std::chrono::steady_clock::now();
        //std::chrono::duration<double> duration = end-start;
        //std::cout << buffer << std::endl;
        //printf(">> Recebi\n");
        //printf("Tempo de pedido-resposta (%d): %f sec\n", client_id, duration.count());
    }
    return 0;
}