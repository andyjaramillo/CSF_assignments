#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "cache_funcs.h"
#include <unistd.h>
#include <math.h>
#include <cstring>
using namespace std;

int total_Loads = 0;

int total_Stores = 0;

int load_Hits = 0;

int load_Misses = 0;

int store_Hits =0;

int store_Misses =0;

int Total_Cycles=0;

int cache_timestamp = 0;

    void writeMethod(std::string parameter , Slot * slot, Cache& cache){
        if(parameter.compare("write-through") == 0){
            write_through(cache, slot);
          
        }else if(parameter.compare("write-allocate")  == 0){
            write_allocate(cache, slot);
        }else if(parameter.compare("no-write-allocate") == 0){
            no_write_allocate(cache, slot);
           
           
        }else if(parameter.compare("write-back")  == 0){
            write_back(cache, slot);
            if(slot->dirty_bit == true){
                total_Stores++;
            }
           
        }
    }
    void Load_hitOrMiss(Cache& cache, Slot * s , std::string first_string , std::string second_string, int byte_size, std::string lru_or_fifo){
     if(slotExists(cache, s)){
            //hit
            load_Hits++;
            Total_Cycles++;
             Set currentSet =  cache.sets[s->index];
            for (long unsigned i=0; i < currentSet.slots.size(); i++)
                 {
            Slot currentSlot = currentSet.slots[i];
            if (currentSlot.tag == s->tag) {
                cache.sets[s->index].slots[i].access_ts++;           
                break;
                  }
             }
           // writeMethod(second_string, s, cache);
        }else{
            //miss
            load_Misses++;
            Total_Cycles += (100* (byte_size/4));
            if(isCacheFull(cache, s) == true){
                if(lru_or_fifo.compare("lru")){
                    //To do 
                }
                // }else if(lru_or_fifo.compare("fifo")){
                    
                // }
            }else{
                //no eviction needed
                
                
            }
         //   writeMethod(first_string, s, cache);
        }
        
}
void Store_hitOrMiss(Cache& cache, Slot * s , std::string first_string , std::string second_string){
    //  int& total_store = return_total_Stores();
     s->access_ts++;
     if(slotExists(cache, s)){
            //hit
           store_Hits++;
           //total_store++;
        }else{
            //miss
            store_Misses++;
           // total_store++;
            if(isCacheFull(cache, s)!=true){
             //    Load_hitOrMiss(cache,s,first_string,second_string);

            }else{
                evictionFunction(cache, s);
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
int main(int argc , char * argv[]){
     char buffer[15] = {0};
    Cache cache(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
   
    int number_of_bytes= read(0, buffer, 15);
     cache_timestamp++;
    // if(argc < 7){
    //     cerr << "Missing File" << endl;
    //     return 0;
    // }
  
    while(number_of_bytes>=14){
        string line_as_string = convertToString(buffer, 15);
        vector<string> line_as_vector = to_Vector(line_as_string);
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
        uint64_t val = strtol(char_array, &end, 16);
        uint32_t offset_bits =  atoi(argv[3]) /4;
        uint32_t bitmask= pow(2,offset_bits)-1;
      //  cout << bitmask << " ";
     
        uint32_t offset = val & bitmask;
        val = val >> offset_bits;
        uint32_t index_shift = (3* atoi(argv[3]))/8;
        bitmask = pow(2,index_shift)-1; 

        uint32_t index = val & bitmask;
        
        val = val >> index_shift;
        uint32_t tag_shift = (3* atoi(argv[3]))/8;
        bitmask = pow(2, tag_shift)-1;
       
        uint32_t tag = val & bitmask;
      
        Slot slot;
        slot.tag = tag;
        slot.index = index;
   //  cout << slot.offset << " ";
        string action = line_as_vector[0];
        if(action == "l"){
            total_Loads++;
            Load_hitOrMiss(cache, &slot, argv[4], argv[5], atoi(argv[3]), argv[6]);
        }else if(action == "s"){
            //check if it is a hit or miss because we want to see if we can read from memory
            total_Stores++;
            Store_hitOrMiss(cache, &slot, argv[4], argv[5]);
           
        }
        
        writeMethod(argv[4], &slot, cache);

      //  writeMethod(argv[5], &slot, cache);
        
        /*
        reading next line in the file
        */
        number_of_bytes= read(0, buffer, 15); 
         cache_timestamp++;
     
    }
  // print_Cache(cache);


  cout << "Total loads: " << total_Loads << endl;
  cout << "Total stores: " << total_Stores << endl;
  cout << "Load Hits: " << load_Hits << endl;
  cout << "Load misses: " << load_Misses << endl;
  cout << "Store hits: " << store_Hits << endl;
  cout << "Store misses: " << store_Misses << endl;
  cout << "Total cycles: " << Total_Cycles << endl;

    return 0;
   
}

