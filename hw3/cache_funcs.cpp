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


bool slotExists(Cache& ca, Slot *s){
    // auto i = cache_map.find(s->tag);
    //     if(i != cache_map.end()){
    //         return true;
    //     }else{
    //         return false;
    //     }
        for (long unsigned i=0; i < ca.sets[s->index].slots.size(); i++)
        {
            if (ca.sets[s->index].slots[i].valid ==true && ca.sets[s->index].slots[i].tag == s->tag) {
                return true;
            }
        }
        return false;
}

bool isCacheFull(Cache& ca, Slot * s){

    Set currentSet =  ca.sets[s->index];
    for(long unsigned i=0; i < currentSet.slots.size(); i++){
        if(currentSet.slots[i].valid == false){
            return false;
        }
    }
        return true;
}

void evictionFunction(Cache& ca, Slot * s, int timestamp, int byte_size){
    Set currentSet =  ca.sets[s->index];
    uint32_t min = currentSet.slots[0].access_ts;
    int index = 0;
        for(long unsigned i=1; i< currentSet.slots.size(); i++){
        if(min >= currentSet.slots[i].access_ts){
            min = currentSet.slots[i].access_ts;
            index = i;
        }
        }
        if(ca.sets[s->index].slots[index].dirty_bit){
             ca.total_cycles += ca.byte_size_calculation; 
        }
        ca.sets[s->index].slots[index].tag = s->tag;
        ca.sets[s->index].slots[index].access_ts = timestamp;
        ca.sets[s->index].slots[index].dirty_bit = false;
        ca.sets[s->index].slots[index].valid = true;
        cache_map[s->tag] = s;
    
}


//a store writes to the cache as well as to memory
void write_through(Cache &ca, Slot * s, int timestamp, bool dirty){
    // s->access_ts++;
        Set currentSet =  ca.sets[s->index];
        for (long unsigned i=0; i < currentSet.slots.size(); i++)
        {
            Slot currentSlot = currentSet.slots[i];
            if (currentSlot.valid == false) {
                ca.sets[s->index].slots[i].tag = s->tag;
                ca.sets[s->index].slots[i].access_ts = timestamp;
                ca.sets[s->index].slots[i].valid = true;
                if(dirty){
                    ca.sets[s->index].slots[i].dirty_bit = true;
                }
                cache_map[s->tag] = s;
                break;
            }
        }
       // ca.total_cycles += ca.byte_size_calculation;
        return;
     
    
}
