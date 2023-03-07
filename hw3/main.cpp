#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "cache_funcs.h"
#include <unistd.h>
using namespace std;

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
         char* char_array;
        string hex_string = line_as_vector[1];
        for(long unsigned i=0; i< length; i++){
            char_array+= hex_string[i];
        }
        char_array+=0;
        uint64_t val = strtol(char_array, &end, 16);
        cout << val << endl;
        
        /*
        reading next line in the file
        */
        number_of_bytes= read(0, buffer, 15); 
         
     
    }

    return 0;
   
}
bool string_compare(std::string one, std::string two){
   bool different = false;
    for(long unsigned i=0; i < one.length(); i++){
        if(one[i] != two[i]){
            different = true;
        }
    }
    return different;
}
 void functionCall(string parameter){
        if(string_compare(parameter, "write-through")){

        }else if(string_compare(parameter, "write-allocate")){

        }else if(string_compare(parameter, "no-write-allocate")){

        }else if(string_compare(parameter, "write-back")){

        }
    }
