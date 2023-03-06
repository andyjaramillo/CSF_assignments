#include "cache_funcs.h"
#include "Cache.h"
#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;


string convertToString(char * char_array, int size){
    string s;
    for(int i=0;i<size;i++){
        s= s + char_array[i];
    }
    return s;
}

std::vector<std::string> to_Vector(std::string s){
    stringstream ss(s);
    vector<string> line_of_cache;
    string word;
    while(ss >> word){
        line_of_cache.push_back(word);
    }
    return line_of_cache;
}

//a store writes to the cache as well as to memory
int write_through(Cache ca, Slot s){

    return 0;
}

//a cache miss during a store does not modify the cache
int no_write_allocate(Cache ca, Slot s){

    return 0;
}
