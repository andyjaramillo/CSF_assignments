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

void evictionFunction(Cache& ca, Slot * s, int timestamp, int byte_size){
    Set currentSet =  ca.sets[s->index];
    uint32_t min = currentSet.slots[0].access_ts;
    int index = 0;
    if(s->dirty_bit == true){
         for(long unsigned i=0; i< currentSet.slots.size(); i++){
        if(ca.sets[s->index].slots[i].tag == s->tag){
           ca.total_cycles += ca.byte_size_calculation;         //or add by 100 not sure
           ca.sets[s->index].slots[i].tag = s->tag;
           ca.sets[s->index].slots[i].access_ts = timestamp;
           ca.sets[s->index].slots[i].dirty_bit = false;
        }
    }
    }else{
        for(long unsigned i=1; i< currentSet.slots.size(); i++){
        if(min > currentSet.slots[i].access_ts){
            min = currentSet.slots[i].access_ts;
            index = i;
        }
        }
        ca.sets[s->index].slots[index].tag = s->tag;
        ca.sets[s->index].slots[index].access_ts = timestamp;
        ca.sets[s->index].slots[index].dirty_bit = false;
    }
    
}

// void write_no_dirty_through(Cache& ca , Slot * s, int timestamp){
    
    
// }

// void write_no_dirty_allocate(Cache& ca , Slot * s, int timestamp){
    
//     s->access_ts++;
//         Set currentSet =  ca.sets[s->index];
//        for (long unsigned i=0; i < currentSet.slots.size(); i++)
//         {
//             Slot currentSlot = currentSet.slots[i];
//             if (currentSlot.tag == 0) {
//                 currentSlot.tag = s->tag;
//                 ca.sets[s->index].slots[i].tag = currentSlot.tag;
//                 cache_map.insert({s->tag, s});
//                 break;
//             }
//         }
//         return;
    

// }

//a store writes to the cache as well as to memory
void write_through(Cache& ca, Slot * s, int timestamp){
     s->access_ts++;
        Set currentSet =  ca.sets[s->index];
        for (long unsigned i=0; i < currentSet.slots.size(); i++)
        {
            Slot currentSlot = currentSet.slots[i];
            if (currentSlot.tag == 0) {
                ca.sets[s->index].slots[i].tag = s->tag;
                ca.sets[s->index].slots[i].access_ts = timestamp;
                cache_map[s->tag] = s;
                break;
            }
        }
        return;
     
    
}

// //a cache miss during a store does not modify the cache
// void no_write_allocate(Cache ca, Slot * s){
//     return;
// }

void write_back(Cache ca, Slot * s, int timestamp){
  //  load_Hits++;
  //  total_Loads++;
    
    s->access_ts++;
    // if(cache_map.count(s->tag)){
    //     Set currentSet =  ca.sets[s->index];
    //     for (long unsigned i=0; i < currentSet.slots.size(); i++)
    //     {
    //         Slot currentSlot = currentSet.slots[i];
    //         if (currentSlot.tag == s->tag) {
    //             ca.sets[s->index].slots[i].tag = currentSlot.tag;
    //             cache_map[s->tag] = s;
    //             break;
    //         }
    //     }
    //     return;
   
        Set currentSet =  ca.sets[s->index];
       for (long unsigned i=0; i < currentSet.slots.size(); i++)
        {
            Slot currentSlot = currentSet.slots[i];
            if (currentSlot.tag == 0) {
                ca.sets[s->index].slots[i].tag = s->tag;
                ca.sets[s->index].slots[i].dirty_bit = true;
                cache_map.insert({s->tag, s});
                break;
            }
        }
        return;
    

}

// void write_allocate(Cache ca, Slot * s){
//     //only writes to cache
//     write_no_dirty_through(ca, s);
// }


