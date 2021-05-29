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
#include <atomic>
#include <sys/stat.h>
#include "hash.h"

#define PORT    6000            // port
#define MEMORIA 104857600       // bytes == 100 MB
#define DISCO   1073741824      // bytes == 1 GB
#define N       1024            // array size

/* 100 megas para memória e 1G para disco */

std::mutex mtx_file;
//std::atomic<long> count(0);
//std::atomic<long> gcount(0);

std::fstream ifile;
std::ifstream rfile("db.txt");


/* Devolve registo que esteja em disco com determinada key */
std::string file_get(long long key, int hash_size)
{
    std::string line;
    size_t pos = 20;

    mtx_file.lock();
    
    rfile.seekg(((key % (hash_size + DISCO/(1024+20+1+1)))- hash_size)*(1024+20+1+1));
    getline(rfile, line);
    
    mtx_file.unlock();
    
    line.erase(0, pos + std::string(" ").length()); 
    pos = line.find("\n");
    std::string value = line.substr(0, line.find("\n")).c_str();

    return value;
}

/* Insere registo em disco */
std::string file_put(long long key, std::string value, int hash_size)
{
    std::string line;
    size_t pos = 20;
    int op = 0;

    mtx_file.lock();
    
    ifile.seekg(((key % (hash_size + DISCO/(1024+20+1+1)))- hash_size)*(1024+20+1+1));
    getline(ifile,line);
    std::string line_aux = line.substr(0, pos);
    remove(line_aux.begin(), line_aux.end(), ' ');
    std::stringstream toINT(line_aux);
    toINT >> op;
    long x = line.length()+1;
    long t = (long)ifile.tellg() - x;
    ifile.seekp(t);
    ifile.clear();
    ifile << std::setfill(' ') << std::setw(20) << key << " " << value << "\n";
    
    mtx_file.unlock();
    
    return "0";
}

/* Handle da resposta ao pedido do cliente */
std::string handle_query(hash::Hash h, char option[N*N])
{
    std::string query(option); 
    size_t pos = query.find("\n");
    std::stringstream toINT(query.substr(0, pos));              // option
    int op = 0;
    toINT >> op;
    query.erase(0, pos + std::string("\n").length()); 

    pos = query.find("\n");
    try{
        long long key = std::stol(query.substr(0, pos)) % (MEMORIA/1024+8 + DISCO/(1024+20+1+1)) ;        // key
        //std::cout << "Key: " << key  << "\n";
        if(op == 0){
            if(h.size() > key)
            {
                //auto start = std::chrono::steady_clock::now();
                std::string res = std::string(h.getElem(key));
                //auto end = std::chrono::steady_clock::now();
                //std::chrono::duration<double> duration = end-start;
                //printf("g %f\n", duration.count());
                return res;
            }
            else if(key < h.size() + DISCO/(1024+20+1+1)){
                //auto start = std::chrono::steady_clock::now();
                std::string value = file_get(key, h.size());
                //auto end = std::chrono::steady_clock::now();
                //std::chrono::duration<double> duration = end-start;
                //printf("g %f\n", duration.count());
                return value;
            }
            else{
                return "0";
            }
        }
        else{
            query.erase(0, pos + std::string("\n").length());
            pos = query.find("\n");
            std::string value = query.substr(0, query.find("\n"));  // value
            if(h.size() > key){
                //auto start = std::chrono::steady_clock::now();
                h.putElem(hash::Hash_Elem(key, value));
                //auto end = std::chrono::steady_clock::now();
                //std::chrono::duration<double> duration = end-start;
                //printf("p %f\n", duration.count());
                return "1";
            }
            else{
                //auto start = std::chrono::steady_clock::now();
                std::string res = file_put(key, value, h.size());
                //auto end = std::chrono::steady_clock::now();
                //std::chrono::duration<double> duration = end-start;
                //printf("p %f\n", duration.count());
                return res;
            }
        }
    }
    catch(const std::out_of_range)
    {
        return "0";
    }
}

/* Handle da conexão com o cliente */
void handle_client(hash::Hash h, int sock, unsigned t_id){
    int val;
    char buffer[N*N] = {0};

    //auto start = std::chrono::steady_clock::now();
    //auto end = std::chrono::steady_clock::now();

    //std::chrono::duration<double> duration = end - start;
    

    while(true){
        //duration = end-end;
        //start = std::chrono::steady_clock::now();
        //while (duration.count() < 1.f){
            bzero(buffer, N*N);
            val = read(sock, buffer, N*N);
            if(val == 0)
                    continue;
            if(val < 0 )
                    break;
            //count++;
            //gcount++;
            strcpy(buffer, handle_query(h, buffer).c_str());
            send(sock, buffer, strlen(buffer), 0);
            //end = std::chrono::steady_clock::now();
            //duration = end-start;
            //if(gcount.load() >= 1000000) exit(0);
        //}
        //printf("%f\n", count.load() / duration.count());
        //if( t_id == 0 )
        //{
        //    //printf("(%d) Número de pedidos por segundo: %lu && %f (avg = %f)\n", t_id, count.load(), duration.count(), count.load() / duration.count());
        //    //printf("%f\n", count.load() / duration.count());
        //    count.store(0);
        //} 
    }
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
    if(!file_exists("db.txt")){
        std::ofstream File("db.txt", std::ios::trunc);
        for(int i = 0; i < tam; i++){
            std::string s = hash::random_string(N);
            File << std::setfill(' ') << std::setw(20) << key++ << " " << s << "\n";
        }
        File.close();
    }

    //printf("-- DADOS CARREGADOS --\n");
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

    /* Abrir uma filestream para o ficheiro */

    ifile.open("db.txt", std::ios::in |std::ios::out);

    /* Aceita conexão */

    unsigned t_id = 0;
    
    while(true){
        sock = accept(fd, (struct sockaddr *)&address, (socklen_t*)&address_len);
        if(sock < 0)
            perror("accept falhou\n");    
        threads.push_back(std::thread(&handle_client, h, sock, t_id));
        t_id++;
    }

    for(auto &t: threads)
        t.join();
    return 0;
}