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

using std::isdigit;
using namespace std;

uint32_t total_Loads = 0;

uint32_t total_Stores = 0;

uint32_t load_Hits = 0;

uint32_t load_Misses = 0;

uint32_t store_Hits =0;

uint32_t store_Misses =0;



uint32_t cache_timestamp = 0;




    // void writeMethod(std::string parameter , Slot * slot, Cache& cache){
    //     if(parameter.compare("write-through") == 0){
    //         write_through(cache, slot, cache_timestamp);
          
    //     }else if(parameter.compare("write-allocate")  == 0){
    //         write_allocate(cache, slot);
    //     }else if(parameter.compare("no-write-allocate") == 0){
    //         no_write_allocate(cache, slot);
           
           
    //     }else if(parameter.compare("write-back")  == 0){
    //         write_back(cache, slot);
    //         if(slot->dirty_bit == true){
    //             total_Stores++;
    //         }
           
    //     }
    // }
    void Load_hitOrMiss(Cache& cache, Slot * s , std::string first_string , std::string second_string, int byte_size, std::string lru_or_fifo){
     if(slotExists(cache, s)){
            //hit
            load_Hits++;
            cache.total_cycles++;
             Set currentSet =  cache.sets[s->index];
            for (long unsigned i=0; i < currentSet.slots.size(); i++)
                 {
            Slot currentSlot = currentSet.slots[i];
            if (currentSlot.tag == s->tag) {
                cache.sets[s->index].slots[i].access_ts = cache_timestamp;           
                break;
                  }
             }
           // writeMethod(second_string, s, cache);
        }else{
            //miss
            load_Misses++;
            cache.total_cycles += cache.byte_size_calculation;
            if(isCacheFull(cache, s) == true){
                if(lru_or_fifo.compare("lru")){
                    //To do 
                    evictionFunction(cache, s, cache_timestamp, byte_size);
                }
                // }else if(lru_or_fifo.compare("fifo")){
                    
                // }
            }else{
                //no eviction needed
                 if(second_string.compare("write-through")){
                     write_through(cache, s, cache_timestamp);
                    
                 }else{
                     write_back(cache, s, cache_timestamp);
                 }
                    
            }
        }
        
}
void Store_hitOrMiss(Cache& cache, Slot * s , std::string first_string , std::string second_string){
    //  int& total_store = return_total_Stores();
     s->access_ts++;
     if(slotExists(cache, s)){
            //hit
           store_Hits++;
            if(first_string.compare("write-through")){
                write_through(cache, s, cache_timestamp);
            } else{
                write_back(cache, s, cache_timestamp);
            }
            
          
        }else{
            //miss
            store_Misses++;
            if(first_string.compare("write_allocate")){
                if(second_string.compare("write-through")){
                    write_through(cache,s,cache_timestamp);
                    cache.total_cycles += 100;
                }else{
                    write_back(cache, s, cache_timestamp);
                }
            } else{ //write no allocate
                cache.total_cycles += 100;
            }
            
          
        }
        
}

    void print_Cache(Cache ca){
         for(int i=0; i<ca.num_sets; i++){
            for(int j=0; j< ca.num_slots; j++){
               if(ca.sets[i].slots[j].tag != 0){
                    cout <<  ca.sets[i].slots[j].tag << " ";
               }
                
            }
          //  cout << endl;
        }
    }

bool is_a_power_of_two(int x){
    return (x!=0) && ((x & (x - 1)) == 0);
}
bool is_an_integer(string s){
   long unsigned counter =0;
    while(counter != s.length()){
        if(!isdigit(s[counter])){
            return false;
        }
        counter++;
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
    if(strcmp(argv[4],"write-allocate") !=0 && strcmp(argv[4],"no-write-allocate") != 0){
        cerr << "4th parameter must be write-allocate or no-write-allocate" << endl;
        return 1;
    }
    if(strcmp(argv[5],"write-through") !=0 && strcmp(argv[5],"write-back") != 0){
        cerr << "4th parameter must be write-allocate or no-write-allocate" << endl;
        return 1;
    }
    if(strcmp(argv[6],"lru") != 0 && strcmp(argv[6],"fifo") != 0){
        cerr << "6th parameter must have lru of fifo" << endl;
        return 1;
    }
    
    if(strcmp(argv[4],"no-write-allocate") == 0 && strcmp(argv[5],"write-back") == 0){
        cerr << "Pair of no-write-allocate and write-back is not possible" << endl;
        return 1;
    }

    Cache cache(stoi(argv[1]), stoi(argv[2]), stoi(argv[3]));
   
    //int number_of_bytes= read(0, buffer, 15);
     cache_timestamp++;
    string line;
    while(getline(cin,line)){
       // string line_as_string = convertToString(buffer, 15);
        vector<string> line_as_vector = to_Vector(line);
        /*
        line_as_vector[1] holds the hex string, which is a uint32_t value
        */
        long unsigned length = line_as_vector[1].length();
        char * end;
         char* char_array = new char[length+1];
        string hex_string = line_as_vector[1];
        for(long unsigned i=0; i< length; i++){
            char_array[i] =  hex_string[i];
        }
        char_array[length+1] = 0;
       // cout << char_array << endl;
        uint64_t tag = strtol(char_array, &end, 16);
        tag >>= log2(stoi(argv[3]));
        uint32_t index = tag % stoi(argv[1]);
        tag >>= log2(stoi(argv[1]));
    //     uint32_t offset_bits =  stoi(argv[3]) /4;
    //     uint32_t bitmask= pow(2,offset_bits)-1;
    //   //  cout << bitmask << " ";
     
    //     uint32_t offset = val & bitmask;
    //     val = val >> offset_bits;
    //     uint32_t index_shift = (3* stoi(argv[3]))/8;
    //     bitmask = pow(2,index_shift)-1; 

    //     uint32_t index = val & bitmask;
        
    //     val = val >> index_shift;
    //     uint32_t tag_shift = (3* stoi(argv[3]))/8;
    //     bitmask = pow(2, tag_shift)-1;
       
    //     uint32_t tag = val & bitmask;
      
        Slot slot;
        slot.tag = tag;
        slot.index = index;
   //  cout << slot.offset << " ";
        string action = line_as_vector[0];
        if(action == "l"){
            total_Loads++;
            Load_hitOrMiss(cache, &slot, argv[4], argv[5], stoi(argv[3]), argv[6]);
        }else if(action == "s"){
            //check if it is a hit or miss because we want to see if we can read from memory
            total_Stores++;
            Store_hitOrMiss(cache, &slot, argv[4], argv[5]);
           
        }
        
       // writeMethod(argv[4], &slot, cache);

      //  writeMethod(argv[5], &slot, cache);
        
        /*
        reading next line in the file
        */
       delete char_array;
      //  number_of_bytes= read(0, buffer, 15); 
         cache_timestamp++;
     
    }
  // print_Cache(cache);

  cout << "Total loads: " << total_Loads << endl;
  cout << "Total stores: " << total_Stores << endl;
  cout << "Load Hits: " << load_Hits << endl;
  cout << "Load misses: " << load_Misses << endl;
  cout << "Store hits: " << store_Hits << endl;
  cout << "Store misses: " << store_Misses << endl;
  cout << "Total cycles: " << cache.total_cycles << endl;

    return 0;
   
}

