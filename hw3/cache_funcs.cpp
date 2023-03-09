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

int total_Loads = 0;

int total_Stores = 0;

int load_Hits = 0;

int load_Misses = 0;

int store_Hits =0;

int store_Misses =0;

int Total_Cycles=0;

int& return_total_Loads(){
   return total_Loads; 
}
int& return_total_Stores(){
   return total_Stores; 
}
int& return_load_Misses(){
   return load_Misses; 
}
int& return_store_Hits(){
   return store_Hits; 
}
int& return_store_Misses(){
   return store_Misses; 
}
int& return_total_cycles(){
   return Total_Cycles; 
}


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

bool slotExists(Cache& ca, Slot *s){
    Set currentSet =  ca.sets[s->index];
        for (long unsigned i=0; i < currentSet.slots.size(); i++)
        {
            Slot currentSlot = currentSet.slots[i];
            if (currentSlot.tag == s->tag) {
                return true;
            }
        }
        return false;
}

bool isCacheFull(Cache& ca, Slot * s){
    bool cache_is_full = true;
    for(int i=0; i<ca.num_sets; i++){
            for(int j=0; j< ca.num_slots; j++){
               if(ca.sets[i].slots[j].offset == 0 && ca.sets[i].slots[j].tag == 0){
                  cache_is_full = false;
                  return cache_is_full;
               }
                
            }
        }
        return cache_is_full;
}


//a store writes to the cache as well as to memory
void write_through(Cache& ca, Slot * s){
    // for(auto i : cache_map){
    //     cout << i.first << " " << i.second; 
    // }
    // cout << endl;
    
    if(cache_map.count(s->tag)){
        Set currentSet =  ca.sets[s->index];
        for (long unsigned i=0; i < currentSet.slots.size(); i++)
        {
            Slot currentSlot = currentSet.slots[i];
            if (currentSlot.tag == s->tag) {
                currentSlot.offset = s->offset;
                ca.sets[s->index].slots[i].offset = currentSlot.offset;
                cache_map[s->tag] = s;
                break;
            }
        }
        return;
    }else{
        Set currentSet =  ca.sets[s->index];
       for (long unsigned i=0; i < currentSet.slots.size(); i++)
        {
            Slot currentSlot = currentSet.slots[i];
            if (currentSlot.tag == 0) {
                currentSlot.tag = s->tag;
                ca.sets[s->index].slots[i].tag = currentSlot.tag;
                currentSlot.offset = s->offset;
                ca.sets[s->index].slots[i].offset = currentSlot.offset;
                cache_map.insert({s->tag, s});
                break;
            }
        }
        return;
    }

    
}

//a cache miss during a store does not modify the cache
void no_write_allocate(Cache ca, Slot * s){
    load_Misses++;
    total_Loads++;  
}

void write_back(Cache ca, Slot * s){
    write_through(ca,s);
    s->dirty_bit = true;
}

void write_allocate(Cache ca, Slot * s){
    write_through(ca,s);
    
}
