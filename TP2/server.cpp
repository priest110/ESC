#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include "hash.h"

#define PORT    6000            // port
#define MEMORIA 104857600       // bytes == 100 MB
#define DISCO   1073741824      // bytes == 1 GB
#define N       1024            // array size

/* Devolve registo que esteja em disco com determinada key */
std::string file_get(long long key){
    //long long h_key = hash::Hash::hashFunction(key);
    std::ifstream File("file.txt");
    std::string line;
    while (getline(File,line)){
        size_t pos = line.find(" ");
        std::stringstream toINT(line.substr(0, pos));
        int op = 0;
        toINT >> op;
        if(op == key){
            line.erase(0, pos + std::string(" ").length()); 
            pos = line.find("\n");
            std::string value = line.substr(0, line.find("\n")).c_str();
            File.close();
            return value;
        }
    }
    File.close();
    return "-- A chave inserida não existe --";
}

/* Insere registo em disco */
std::string file_put(long long key, std::string value){
    std::fstream file;
    //long long h_key = hash::Hash::hashFunction(key);
    file.open("file.txt", std::ios::in |std::ios::out);
    std::string line;
    while (getline(file,line))
    {
        size_t pos = line.find(" ");
        std::stringstream toINT(line.substr(0, pos));
        int op = 0;
        toINT >> op;
        if(op == key)
        {
            long x = line.length()+1;
            long t = file.tellg() - x;
            file.seekp(t);
            t = file.tellp();
            file.clear();
            file << key << " " << value << "\n";
            file.close();
            return "-- PUT realizado com sucesso --";
        }
    }
    file.close();
    return "-- A chave inserida não existe --";
}

/* Handle da resposta ao pedido do cliente */
std::string handle_query(hash::Hash h, char option[N*N]){
    std::string query(option); 
    size_t pos = query.find("\n");
    std::stringstream toINT(query.substr(0, pos));              // option
    int op = 0;
    toINT >> op;
    query.erase(0, pos + std::string("\n").length()); 

    pos = query.find("\n");
    try
    {
        long long key = h.hashFunction(std::stol(query.substr(0, pos)));        // key
        if(op == 0){
            if(h.size() > key)
                return std::string(h.getElem(key));
            else
                return file_get(key);
        }
        else{
            query.erase(0, pos + std::string("\n").length());
            pos = query.find("\n");
            std::string value = query.substr(0, query.find("\n"));  // value
            if(h.size() > key){
                h.putElem(hash::Hash_Elem(key, value));
                return "-- PUT realizado com sucesso --";
            }
            else
                return file_put(key, value);
        }
    }
    catch(const std::out_of_range)
    {
        return "OUT OF RANGE";
    }
}

/* Handle da conexão com o cliente */
void handle_client(hash::Hash h, int sock){
    int val;
    char buffer[N*N] = {0};
    while (true){
        bzero(buffer, N*N);
        val = read(sock, buffer, N*N);
        if(val == 0)                        // determinado cliente saiu
            continue;
        if(val < 0 )
            break;
        printf("Mensagem recebida: %s\n", buffer);
        strcpy(buffer, handle_query(h, buffer).c_str());
        send(sock, buffer, strlen(buffer), 0);
    }
}

/* Carrega dados para memória e disco */
hash::Hash data(){
    int tam = MEMORIA / 1032;               // tam = 101606 registos em memória
    hash::Hash h(tam);
    long long key = 0;
    for(int i = 0; i < tam; i++){
        std::string s = hash::random_string(N); // está a pôr um ! no fim
        h.putElem(hash::Hash_Elem(key++, s));
    }

    //h.show();

    tam = DISCO/1032;                       // tam = 1040447 registos em disco
    
    std::ofstream File("file.txt", std::ios::trunc);
    for(int i = 0; i < tam; i++){
        std::string s = hash::random_string(N);
        File << key++ << " " << s << "\n";
    }
    File.close();

    printf("-- DADOS CARREGADOS --\n");
    return h;
}

int main(int argc, char *argv[]){
    int fd, sock;
    struct sockaddr_in address;             // structure for handling internet addresses
    int address_len = sizeof(address);
    std::vector<std::thread> threads;

    /* Criação da socket, get do fd */
    fd = socket(AF_INET, SOCK_STREAM, 0);   // domain, type, protocol
    if(fd == 0)
        perror("criação da socket falhou");

    /* (opcional) Ajuda no reutilização do endereço ou da porta. Previne erros como "address already in use" */
    int opt = 1;
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        perror("setsockopt");

    /* Setup do address para o bind */
    address.sin_family = AF_INET;           // Server byte order
    address.sin_addr.s_addr = INADDR_ANY;   // Automatically be filled with current host's IP address
    address.sin_port = htons(PORT);         // Convert short integer value for port must be converted into network byte order
    
    /* Bind da socket */
    if(bind(fd, (struct sockaddr *)&address, address_len) < 0)
        perror("bind falhou");
    
    /* Começa a ouvir */
    if(listen(fd, 3) < 0)                   // Maximum length 
        perror("listen falhou");

    hash::Hash h = data();

    /* Aceita conexão */
    while(true){
        sock = accept(fd, (struct sockaddr *)&address, (socklen_t*)&address_len);
        if(sock < 0)
            perror("accept falhou");    
        threads.push_back(std::thread(&handle_client, h, sock));
    }

    for(auto &t: threads)
        t.join();
    return 0;
}