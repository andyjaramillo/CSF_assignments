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
        for(auto i: line_as_vector){
            cout << i << " ";
        }
        cout << endl;
        

        number_of_bytes= read(0, buffer, 15); 
         
     
    }

    return 0;
    void functionCall(string parameter){
        if(compare(parameter, "write-through")){

        }else if(compare(parameter, "write-allocate")){

        }else if(compare(parameter, "no-write-allocate")){

        }else if(compare(parameter, "write-back")){

        }
    }
}