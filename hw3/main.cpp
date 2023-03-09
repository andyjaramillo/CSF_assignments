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


    void writeMethod(std::string parameter , Slot * slot, Cache& cache){
        if(parameter.compare("write-through") == 0){
            write_through(cache, slot);
        }else if(parameter.compare("write-allocate")  == 0){
            write_allocate(cache, slot);
        }else if(parameter.compare("no-write-allocate") == 0){
            no_write_allocate(cache, slot);
        }else if(parameter.compare("write-back")  == 0){
            write_back(cache, slot);
        }
    }
    void Load_hitOrMiss(Cache& cache, Slot * s , std::string first_string , std::string second_string){
     if(slotExists(cache, s)){
            //hit
            writeMethod(second_string, s, cache);
        }else{
            //miss
            writeMethod(first_string, s, cache);
        }
        
}
void Store_hitOrMiss(Cache& cache, Slot * s , std::string first_string , std::string second_string){
     int& total_store = return_total_Stores();
     if(slotExists(cache, s)){
            //hit
           int& store_hit = return_store_Hits();
           store_hit++;
           total_store++;
        }else{
            //miss
            int& store_miss = return_store_Misses();
            store_miss++;
            total_store++;
           Load_hitOrMiss(cache,s,first_string,second_string);
        }
        
}

    void print_Cache(Cache ca){
         for(int i=0; i<ca.num_sets; i++){
            for(int j=0; j< ca.num_slots; j++){
               if(ca.sets[i].slots[j].offset != 0 || ca.sets[i].slots[j].tag != 0){
                    cout <<  ca.sets[i].slots[j].offset << " ";
               }
                
            }
          //  cout << endl;
        }
    }
int main(int argc , char * argv[]){
     char buffer[15] = {0};
    Cache cache(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    int number_of_bytes= read(0, buffer, 15);
    if(argc < 7){
        cerr << "Missing File" << endl;
        return 0;
    }
  
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
        slot.offset = offset;
   //  cout << slot.offset << " ";
        string action = line_as_vector[0];
        if(action == "l"){
            Load_hitOrMiss(cache, &slot, argv[4], argv[5]);
        }else if(action == "s"){
            //check if it is a hit or miss because we want to see if we can read from memory
            Store_hitOrMiss(cache, &slot, argv[4], argv[5]);
           
        }
        
        writeMethod(argv[4], &slot, cache);

      //  writeMethod(argv[5], &slot, cache);
        
        /*
        reading next line in the file
        */
        number_of_bytes= read(0, buffer, 15); 
         
     
    }
   print_Cache(cache);
    return 0;
   
}

