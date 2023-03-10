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

bool slotExists(Cache& ca, Slot *s){
        for (long unsigned i=0; i < ca.sets[s->index].slots.size(); i++)
        {
            if (ca.sets[s->index].slots[i].tag == s->tag) {
                return true;
            }
        }
        return false;
}

bool isCacheFull(Cache& ca, Slot * s){
    bool cache_is_full = true;
    for(int i=0; i<ca.num_sets; i++){
            for(int j=0; j< ca.num_slots; j++){
               if(ca.sets[i].slots[j].tag == 0){
                  cache_is_full = false;
                  return cache_is_full;
               }
                
            }
        }
        return cache_is_full;
}

void evictionFunction(Cache& ca, Slot * s){
    Set currentSet =  ca.sets[s->index];
    uint32_t min = currentSet.slots[0].access_ts;
    int index = 0;
    for(long unsigned i=1; i< currentSet.slots.size(); i++){
        if(min > currentSet.slots[i].access_ts){
            min = currentSet.slots[i].access_ts;
            index = i;
        }
    }
    ca.sets[s->index].slots[index].tag = s->tag;

}

void write_no_dirty_hit(Cache& ca , Slot * s){
     if(isCacheFull(ca,s)==true){
        evictionFunction(ca,s);
        return;
    }
    s->access_ts++;
    if(cache_map.count(s->tag)){
        Set currentSet =  ca.sets[s->index];
        for (long unsigned i=0; i < currentSet.slots.size(); i++)
        {
            Slot currentSlot = currentSet.slots[i];
            if (currentSlot.tag == s->tag) {
                ca.sets[s->index].slots[i].tag = currentSlot.tag;
                cache_map[s->tag] = s;
                break;
            }
        }
        return;
    }
    

}

void write_no_dirty_miss(Cache& ca , Slot * s){
     if(isCacheFull(ca,s)==true){
        evictionFunction(ca,s);
        return;
    }
    s->access_ts++;
        Set currentSet =  ca.sets[s->index];
       for (long unsigned i=0; i < currentSet.slots.size(); i++)
        {
            Slot currentSlot = currentSet.slots[i];
            if (currentSlot.tag == 0) {
                currentSlot.tag = s->tag;
                ca.sets[s->index].slots[i].tag = currentSlot.tag;
                cache_map.insert({s->tag, s});
                break;
            }
        }
        return;
    

}

//a store writes to the cache as well as to memory
void write_through(Cache& ca, Slot * s){
    // for(auto i : cache_map){
    //     cout << i.first << " " << i.second; 
    // }
    // cout << endl;
   // load_Hits++;
  //  total_Loads++;
    write_no_dirty_hit(ca, s);
    
}

//a cache miss during a store does not modify the cache
void no_write_allocate(Cache ca, Slot * s){
    return;
}

void write_back(Cache ca, Slot * s){
  //  load_Hits++;
  //  total_Loads++;
    if(isCacheFull(ca,s)==true){
        if(s->dirty_bit==true){
            evictionFunction(ca,s);
            
        }else{
            evictionFunction(ca,s);
        }
        return;
    }
    
    s->access_ts++;
    if(cache_map.count(s->tag)){
        Set currentSet =  ca.sets[s->index];
        for (long unsigned i=0; i < currentSet.slots.size(); i++)
        {
            Slot currentSlot = currentSet.slots[i];
            if (currentSlot.tag == s->tag) {
                ca.sets[s->index].slots[i].tag = currentSlot.tag;
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
                ca.sets[s->index].slots[i].tag = s->tag;
                cache_map.insert({s->tag, s});
                break;
            }
        }
        return;
    }
    s->dirty_bit = true;

}

void write_allocate(Cache ca, Slot * s){
  
    write_no_dirty_miss(ca, s);
}


