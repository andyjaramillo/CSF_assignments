#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "cache_funcs.h"
#include <unistd.h>
#include <math.h>
#include <cstring>
#include <ctype.h>
#include <sstream>

using std::isdigit;
using namespace std;

uint64_t total_Loads = 0;

uint64_t total_Stores = 0;

uint64_t load_Hits = 0;

uint64_t load_Misses = 0;

uint64_t store_Hits =0;

uint64_t store_Misses =0;


uint64_t cache_timestamp = 0;

bool is_write_through = false;

bool is_write_allocate = false;

bool is_lru = false;

    int Load_hitOrMiss(Cache &cache, Slot * s , int byte_size, std::string lru_or_fifo){
        Slot * slot = slotExists(cache, s);
     if(slot != NULL){
            //hit
          
            cache.total_cycles++;
            slot->access_ts = cache_timestamp;
           // update_load_ts(cache, s);
           return 1;
        }else{
           
            if(isCacheFull(cache, s) == true){
                    evictionFunction(cache, s, cache_timestamp, byte_size, is_lru);
                     cache.total_cycles += cache.byte_size_calculation;
            }else{
                //no eviction needed
                    update_load_ts(cache, s); 
                     write_through(cache, s, cache_timestamp, false);
                     cache.total_cycles += cache.byte_size_calculation;
                 }
                return -1;

        }
    }
        

int Store_hitOrMiss(Cache &cache, Slot * s, int byte_size){
  
    // s->access_ts++;
     Slot * slot = slotExists(cache, s);
     if(slot != NULL){
            //hit
    
                        slot->access_ts = cache_timestamp;
                        if(is_write_through){
                            slot->dirty_bit = false;
                             cache.total_cycles += 100;
                             cache.total_cycles++;
                        }else{
                            slot->dirty_bit = true;
                            cache.total_cycles++;
                          
                        }
                     //   update_load_ts(cache, s); 
            
          return 1;
        }else{
            //miss
           
            if(is_write_allocate){
                
           
            if(isCacheFull(cache, s) == true){
                    evictionFunction(cache, s, cache_timestamp, byte_size, is_lru);
                     cache.total_cycles += cache.byte_size_calculation;
            }else{
                //no eviction needed
                 if(is_write_through){
                     update_load_ts(cache, s);
                     write_through(cache, s, cache_timestamp, false); 
                     cache.total_cycles += cache.byte_size_calculation;
                     cache.total_cycles += 100;   
                     
                 }else{
                    update_load_ts(cache, s);
                     write_through(cache, s, cache_timestamp, true);
                     cache.total_cycles += cache.byte_size_calculation;
                    
                 }
                    
            }
            } else{ //write no allocate
                cache.total_cycles += 100;
            }
            return -1;
        }
        
}

    void print_Cache(Cache ca){
         for(int i=0; i<ca.num_sets; i++){
            for(int j=0; j< ca.num_slots; j++){
               if(ca.sets[i].slots[j].tag != 0){
                    cout <<  ca.sets[i].slots[j].tag << " ";
               }
                
            }
        
        }
    }

bool is_a_power_of_two(int x){
    return (x!=0) && ((x & (x - 1)) == 0);
}

bool is_an_integer(char* s){
   
    while(*s){
        if(!isdigit(*s)){
            return false;
        }
        s++;
    }
    return true;
}

int log2(int off){
    int result =0;
    while(off >>= 1){
        result++;
    }
    return result;
}

int main(int argc , char * argv[]){
    
/**
Tests if invalid input in some way
*/

    if(argc != 7){
        cerr << "Not enough arguments" << endl;
        return 1;
    }
    if(!is_an_integer(argv[1])){
        cerr << "The number of sets provided by must a power of two" << endl;
        return 1;
    }
    int cache_size = stoi(argv[1]);
    if(!is_a_power_of_two(cache_size) || cache_size <= 0){
        cerr << "Number of sets in a cache must be a power of two" << endl;
        return 1;
    }
    if(!is_an_integer(argv[2])){
        cerr << "The number of slots provided by must a power of two" << endl;
        return 1;
    }
    int set_size = stoi(argv[2]);
    if(!is_a_power_of_two(set_size) || set_size <= 0){
        cerr << "Number of slots in a set must be a power of two" << endl;
        return 1;
    }
    if(!is_an_integer(argv[3])){
        cerr << "The number of slots provided by must a power of two" << endl;
        return 1;
    }
    int slot_size = stoi(argv[3]);
    if(!is_a_power_of_two(slot_size) || slot_size <= 0){
        cerr << "Number of bytes in a slot must be a power of two" << endl;
        return 1;
    }
    if(slot_size < 4){
        cerr << "Block size must be greater than 4" << endl;
        return 1;
    }
    if(strcmp(argv[4],"write-allocate") ==0){
       is_write_allocate = true;
    }
    else if(strcmp(argv[4],"no-write-allocate") ==0){
        is_write_allocate = false;
    }
    else{
        cerr << "wrong 4th input" <<endl;
        return 1;
    }
    
    if(strcmp(argv[5],"write-through") ==0){
        is_write_through = true;
    } else if(strcmp(argv[5],"write-back") == 0){
        is_write_through = false;
    } else{
        cerr << "wrong 5th parameter" << endl;
        return 1;
    }

    if(strcmp(argv[6],"lru") == 0){
        is_lru = true;
    } else if(strcmp(argv[6],"fifo") == 0){
        is_lru = false;
    } else{
        cerr << "wrong type for 6th parameter" << endl;
        return 1;
    }
   
    if(strcmp(argv[4],"no-write-allocate") == 0 && strcmp(argv[5],"write-back") == 0){
        cerr << "Pair of no-write-allocate and write-back is not possible" << endl;
        return 1;
    }

    Cache cache(stoi(argv[1]), stoi(argv[2]), stoi(argv[3]));
     
    cache_timestamp++;
    string line;

    while(getline(cin,line)){
   
        istringstream ss(line);
        char action;
        string address;
        ss >> action;
        ss >> address;
      
        uint32_t tag = stol(address, 0, 16);
        tag >>= log2(stoi(argv[3]));
        uint32_t index = tag % stoi(argv[1]);
        tag >>= log2(stoi(argv[1]));
    
        Slot slot;
        slot.tag = tag;
        slot.index = index;
       
        if(action == 'l'){
            total_Loads++;
            if(Load_hitOrMiss(cache, &slot, stoi(argv[3]), argv[6]) == 1){
                load_Hits++;
            }else{
                load_Misses++;
            }
        }else if(action == 's'){
            //check if it is a hit or miss because we want to see if we can read from memory
            total_Stores++;
            if(Store_hitOrMiss(cache, &slot, stoi(argv[3])) == 1){
                store_Hits++;
            }else{
                store_Misses++;
            }
           
        }
      
         cache_timestamp++;
     
    }

  cout << "Total loads: " << total_Loads << endl;
  cout << "Total stores: " << total_Stores << endl;
  cout << "Load Hits: " << load_Hits << endl;
  cout << "Load misses: " << load_Misses << endl;
  cout << "Store hits: " << store_Hits << endl;
  cout << "Store misses: " << store_Misses << endl;
  cout << "Total cycles: " << cache.total_cycles << endl;

    return 0;
   
}

