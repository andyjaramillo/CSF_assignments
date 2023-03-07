#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "cache_funcs.h"
#include <unistd.h>
#include <math.h>
using namespace std;
bool string_compare(std::string one, std::string two){
   bool different = false;
    for(long unsigned i=0; i < one.length(); i++){
        if(one[i] != two[i]){
            different = true;
        }
    }
    return different;
}
 void writeMethod(string parameter , Slot * slot, Cache cache){
        if(string_compare(parameter, "write-through")){
            write_through(cache, slot);
        }else if(string_compare(parameter, "write-allocate")){

        }else if(string_compare(parameter, "no-write-allocate")){
            no_write_allocate(cache, slot);
        }else if(string_compare(parameter, "write-back")){

        }
    }
    void print_Cache(Cache ca){
         for(int i=0; i<ca.num_sets; i++){
            for(int j=0; j< ca.num_slots; j++){
                cout <<   ca.sets[i].slots[j].tag << " ";
                
            }
            cout << endl;
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
        writeMethod(argv[4], &slot, cache);

        writeMethod(argv[5], &slot, cache);
        
        /*
        reading next line in the file
        */
        number_of_bytes= read(0, buffer, 15); 
         
     
    }
  //  print_Cache(cache);
    return 0;
   
}

