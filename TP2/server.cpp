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
#include <iomanip>
#include <mutex>
#include <sys/stat.h>
#include "hash.h"

#define PORT    6000            // port
#define MEMORIA 104857600       // bytes == 100 MB
#define DISCO   104857600      // bytes == 1 GB
#define N       1024            // array size

/* 100 megas para memória e 1G para disco */

std::mutex mtx_file;

double count = 0;

/* Devolve registo que esteja em disco com determinada key */
std::string file_get(long long key, int hash_size){
    std::ifstream File("file.txt");
    std::string line;
    File.seekg(((key % (hash_size + DISCO/(1024+20+1+1)))- hash_size)*(1024+20+1+1));
    getline(File, line);
    size_t pos = 20;
    line.erase(0, pos + std::string(" ").length()); 
    pos = line.find("\n");
    std::string value = line.substr(0, line.find("\n")).c_str();
    File.close();
    return value;
}

/* Insere registo em disco */
std::string file_put(long long key, std::string value, int hash_size){
    std::fstream file;
    file.open("file.txt", std::ios::in |std::ios::out);
    std::string line;
    file.seekg(((key % (hash_size + DISCO/(1024+20+1+1)))- hash_size)*(1024+20+1+1));
    getline(file,line);
    size_t pos = 20;
    std::string line_aux = line.substr(0, pos);
    remove(line_aux.begin(), line_aux.end(), ' ');
    std::stringstream toINT(line_aux);
    int op = 0;
    toINT >> op;
    mtx_file.lock();
    long x = line.length()+1;
    long t = file.tellg() - x;
    file.seekp(t);
    t = file.tellp();
    mtx_file.unlock();
    file.clear();
    file << std::setfill(' ') << std::setw(20) << key << " " << value << "\n";
    file.close();
    return "-- PUT realizado com sucesso --";
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
    try{
        long long key = std::stol(query.substr(0, pos)) % (MEMORIA/1024+8 + DISCO/(1024+20+1+1)) ;        // key
        std::cout << "Key: " << key  << "\n";
        if(op == 0){
            if(h.size() > key)
                return std::string(h.getElem(key));
            else if(key < h.size() + DISCO/(1024+20+1+1)){

                printf("--- Key não existe ---\n");
                return file_get(key, h.size());
            }
            else{
                return "--- Key não existe ---";
            }
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
                return file_put(key, value, h.size());
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
    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;
    while (duration.count() < 1.f){
        bzero(buffer, N*N);
        val = read(sock, buffer, N*N);
        if(val == 0)                        // determinado cliente saiu
            continue;
        if(val < 0 )
            break;
        mtx.lock();
        count++;
        mtx.unlock();
        printf("Mensagem recebida: %s\n", buffer);
        strcpy(buffer, handle_query(h, buffer).c_str());
        send(sock, buffer, strlen(buffer), 0);
        printf("Mandei\n");
        end = std::chrono::steady_clock::now();
        duration = end-start;
    }
    printf("Número do pedidos por segundo: %f\n", count);
}

/* Verifica se um ficheiro existe */
bool file_exists(const std::string& filename){
    struct stat buf;
    return (stat(filename.c_str(), &buf) == 0);
}

/* Carrega dados para memória e disco */
hash::Hash data(){
    int tam_mem = MEMORIA / (1024+8);                     // tam = 101606 registos em memória
    hash::Hash h(tam_mem);
    long long key = 0;
    for(int i = 0; i < tam_mem; i++){
        std::string s = hash::random_string(N); 
        h.putElem(hash::Hash_Elem(key++, s));
    }

    //h.show();

    int tam = DISCO/ (1024+20+1+1);                       // tam = 1040447 registos em disco
    if(!file_exists("file.txt")){
        std::ofstream File("file.txt", std::ios::trunc);
        for(int i = 0; i < tam; i++){
            std::string s = hash::random_string(N);
            File << std::setfill(' ') << std::setw(20) << key++ << " " << s << "\n";
        }
        File.close();
    }

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
            perror("accept falhou\n");    
        threads.push_back(std::thread(&handle_client, h, sock));
    }

    for(auto &t: threads)
        t.join();
    return 0;
}