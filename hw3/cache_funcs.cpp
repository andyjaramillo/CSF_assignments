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


Slot* slotExists(Cache& ca, Slot *s){
  
        for (long unsigned i=0; i < ca.sets[s->index].slots.size(); i++)
        {
            if (ca.sets[s->index].slots[i].valid ==true && ca.sets[s->index].slots[i].tag == s->tag) {
                return &ca.sets[s->index].slots[i];
            }
        }
        return NULL;
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
bool isCacheEmpty(Cache& ca, Slot * s){

    Set currentSet =  ca.sets[s->index];
    for(long unsigned i=0; i < currentSet.slots.size(); i++){
        if(currentSet.slots[i].valid == true){
            return false;
        }
    }
        return true;
}

void update_load_ts(Cache& ca, Slot * s){
    Set currentSet =  ca.sets[s->index];
    /*
    order values
    0 0 0 0
      |
      v input at the first slot
    3 2 1 0
      |
      v input at second slot
    1 0 3 2 
    the ones on the left side of the inserted increment, the ones on the right side start the decreasing from max to the end
    */
     long unsigned decrease_counter =  currentSet.slots.size() -1;
    long unsigned index = 0;
        for(long unsigned i=0; i < currentSet.slots.size(); i++){
            if(ca.sets[s->index].slots[i].valid == false){
                index = i;
                break;
            }
        }
     if(isCacheEmpty(ca, s)){
         for(long unsigned i=0; i < currentSet.slots.size(); i++){
                ca.sets[s->index].slots[i].load_ts = decrease_counter;
                decrease_counter--;

            }
      
     } else if(ca.sets[s->index].slots[1].valid == false && ca.sets[s->index].slots[0].valid == true){
        for(long unsigned i=0; i < currentSet.slots.size(); i++){
          if(i == index){
            ca.sets[s->index].slots[i].load_ts = 0;
          }  else {
             if(ca.sets[s->index].slots[i].load_ts == (currentSet.slots.size() -1)){
                      ca.sets[s->index].slots[i].load_ts = 1;
              }else{
                     ca.sets[s->index].slots[i].load_ts += 2;
                }   
          }
            
                     

     }
     } else{
        for(long unsigned i=0; i < currentSet.slots.size(); i++){
          if(i == index){
            ca.sets[s->index].slots[i].load_ts = 0;
          }  else {
             ca.sets[s->index].slots[i].load_ts++;   
          }
            
                     

     }
}
}

void evictionFunction(Cache& ca, Slot * s, int timestamp, int byte_size, bool lru_or_fifo){
   if(lru_or_fifo){ 
    //lru
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
        // cache.total_cycles += cache.byte_size_calculation;
        ca.sets[s->index].slots[index].tag = s->tag;
        ca.sets[s->index].slots[index].access_ts = timestamp;
        ca.sets[s->index].slots[index].dirty_bit = false;
        ca.sets[s->index].slots[index].valid = true;
        cache_map[s->tag] = s;
    } else{
        //fifo 
        Set currentSet =  ca.sets[s->index];
        long unsigned index =0;
        for(long unsigned i =0; i< currentSet.slots.size(); i++){
            if( ca.sets[s->index].slots[i].load_ts == (currentSet.slots.size() -1)){
                index = i;
                break;
            }
        }
        if(ca.sets[s->index].slots[index].dirty_bit){
             ca.total_cycles += ca.byte_size_calculation; 
        }
        // cache.total_cycles += cache.byte_size_calculation;
        ca.sets[s->index].slots[index].tag = s->tag;
        ca.sets[s->index].slots[index].access_ts = timestamp;
        ca.sets[s->index].slots[index].dirty_bit = false;
        ca.sets[s->index].slots[index].valid = true;
        ca.sets[s->index].slots[index].load_ts = 0;
        cache_map[s->tag] = s;
       // long unsigned decrease_counter =  currentSet.slots.size() -1;
        for(long unsigned i=0; i < currentSet.slots.size(); i++){
            if(i != index){
                     ca.sets[s->index].slots[i].load_ts++;
        }

    }
}
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
                }else{
                     ca.sets[s->index].slots[i].dirty_bit = false;
                }
                cache_map[s->tag] = s;
                break;
            }
        }
       
        return;
     
    
}
