#include "cache_funcs.h"
#include "Cache.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <map>
using namespace std;
/*
Map to check if the slot exists
*/
std::map<uint32_t, Slot *> cache_map;

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
int write_through(Cache ca, Slot * s){
    for(auto i : cache_map){
        cout << i.first << " "<< i.second;
    }
    cout << endl;
    if(cache_map.count(s->tag)){
       
        cache_map[s->tag] = s;

    }else{
         
        for(int i=0; i<ca.num_sets; i++){
            for(int j=0; j< ca.num_slots; j++){
                if(ca.sets[i].slots[j].index == s->index){
                    ca.sets[i].slots[j].tag = s->tag;
                    ca.sets[i].slots[j].index = s->index;
                    ca.sets[i].slots[j].offset = s->offset;
                    cache_map.insert({s->tag, s});
                    //cache_map[s->tag] = s;
                }
            }
        }
    }

    return 0;
}

//a cache miss during a store does not modify the cache
int no_write_allocate(Cache ca, Slot * s){

    return 0;
}
