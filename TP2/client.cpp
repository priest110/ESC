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

    while(true){
        printf("--- ESCOLHA UMA DAS OPÇÕES ---\n");
        printf("0 - GET\n");
        printf("1 - PUT\n");
        printf("> ");
        bzero(buffer, N*N);
        fgets(buffer, N*N, stdin);
        if(strcmp(buffer, "quit\n") == 0)
            break;
        else if(strcmp(buffer, "0\n") == 0){
            char aux[N];
            printf("> Key: ");
            fgets(aux, N, stdin);
            strcat(buffer, aux);
        }
        else if(strcmp(buffer, "1\n") == 0){
            char aux[N*N];
            printf("Put aleatório? Sim(1) Não(0)\n");
            fgets(aux, N*N, stdin);
            if(strcmp(aux, "1\n") == 0){
                printf("> Key: ");
                bzero(aux, N*N);
                fgets(aux, N*N, stdin);
                strcat(buffer, aux);
                std::string s = hash::random_string(N);
                strcpy(aux, s.c_str());
                strcat(buffer, aux);
            }
            else{
                printf("> Key: ");
                bzero(aux, N*N);
                fgets(aux, N*N, stdin);
                strcat(buffer, aux);
                printf("> Value: ");
                bzero(aux, N*N);
                fgets(aux, N*N, stdin);
                std::string value;
                if(value.length() != N)
                    continue;
                strcat(buffer, aux);
            }
        }
        else continue;
        auto start = std::chrono::steady_clock::now();
        send(sock, buffer, strlen(buffer), 0);
        bzero(buffer, N*N);
        val = read(sock, buffer, N*N);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> duration = end-start;
        printf(">> %s\n", buffer);
        printf("Tempo de pedido-resposta: %f sec\n", duration.count());
    }
    return 0;
}