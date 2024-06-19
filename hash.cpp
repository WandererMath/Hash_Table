#ifndef HASH_CPP
#define HASH_CPP

#include <limits>
#include <string>
#include <iostream>
#include <exception>
// FNV1 hash function

namespace fnv1{
    const unsigned int prime=4294967295;
    const unsigned int initial_hash=0x811C9DC5;

    void printBinary(unsigned int);

    unsigned int multiply(unsigned int a, unsigned int b){
        uint64_t product = static_cast<uint64_t>(a) * static_cast<uint64_t>(b);
        return static_cast<unsigned int>(product % (1ULL << 32));
    }

    unsigned int hash(std::string s){
        unsigned int result=initial_hash;
        for (char b: s){
            result=multiply(result, prime);
            result^=b;
        }
        return result;
    }

    unsigned int hash_N(std::string s, int N){
        unsigned int mask;
        if(N>0 && N<32){
            mask=(1<<N)-1;
            return hash(s)&mask;
        }
        else if(N==32)  return hash(s);
        else throw std::exception();
    }
    
    //////////////////////////////////////////////////////////
    // test funcitons
    void printBinary(unsigned int number) {
        // Handle invalid input (negative numbers can't be represented in binary)
        if (number < 0) {
            std::cout << "Error: Cannot print negative numbers in binary." << std::endl;
            return;
        }

        // Loop through each bit position (32 bits for unsigned int)
        for (int i = 31; i >= 0; i--) {
            // Check if the current bit is set (1) using bitwise AND with 1 << i
            unsigned int bit = (number >> i) & 1;
            std::cout << bit;
        }

        std::cout << std::endl;
    }

    void test(){
        std::string s="Hello World";
        int N;
        std::getline(std::cin, s);
        std::cin>>N;
        unsigned int result=fnv1::hash(s);
        //std::cout<<result<<std::endl;
        //std::cout<<fnv1::hash_N(s, N)<<std::endl;
        fnv1::printBinary(result);
        fnv1::printBinary(fnv1::hash_N(s, N));
    }
}

#endif