#include <list>
#include <iostream>
#include <cstring>
#include <fstream>
#include <mutex>
#include <algorithm>  //for std::generate_n

std::mutex mtx;

namespace hash {
    class Hash_Elem{ 
        long long key;               // 8 bytes
        std::string value;           // 1024 bytes

        public:
            Hash_Elem();
            Hash_Elem(long long key, std::string val);
            std::string toString();
            long long toKey();
    };


    Hash_Elem::Hash_Elem(long long k, std::string  val){
        this->key = k;
        this->value = val;
    }

    std::string Hash_Elem::toString(){
        return this->value;
    }

    long long Hash_Elem::toKey(){
        return this->key;
    }

    class Hash {
        long long SIZE;                                   // nº de entradas da hash
        std::list<Hash_Elem> *table;                // apontador para o array que contém as entradas da hash

        public:
            Hash(int t); 
            void putElem(Hash_Elem b);              // put key
            long long hashFunction(long long key){        // map values to key
                return(key % SIZE);
            }
            std::string getElem(long long key);           // get key
            int size();;                            // hash size
            void show();                            // show table
    };

    Hash::Hash(int t){
        this->SIZE = t;
        table = new std::list<Hash_Elem>[SIZE];
    }

    void Hash::putElem(Hash_Elem b){
        int i = hashFunction(b.toKey());               // get the hash index of key
        mtx.lock();
        if(table[i].size() != 0)
            table[i].pop_back();
        table[i].push_back(b);
        mtx.unlock();
    }

    std::string Hash::getElem(long long key){
        long long h_key = hashFunction(key);
        if(table[h_key].size() != 0)
            return table[h_key].front().toString();
        return NULL;
    }

    int Hash::size(){
        return this->SIZE;
    }

    void Hash::show(){
        for(int i = 0; i <SIZE; i++){
            std::cout << i;
            for(auto x : table[i])
                std::cout << "-> key: " << x.toKey() << ", value: " << x.toString(); 
            std::cout << std::endl;
            break;
        }
    }

    std::string random_string( size_t length )
    {
        auto randchar = []() -> char
        {
            const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[ rand() % max_index ];
        };
        std::string str(length,0);
        std::generate_n( str.begin(), length, randchar );
        return str;
    }
}