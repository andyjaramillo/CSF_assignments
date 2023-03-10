#include <stdio.h>
#include <vector>
#include <stdint.h>
#ifndef CACHE_H
#define CACHE_H
   struct Slot {
        uint32_t tag;
        uint32_t index;
        uint32_t access_ts;
        bool dirty_bit;
        };
    struct Set {
        std::vector<Slot> slots;
        };
class Cache{

  public:
   int num_sets;
 int num_slots;
     std::vector<Set> sets;
        Cache(int number_of_set, int number_of_blocks_in_set, int number_of_bytes_per_memory){
            num_sets = number_of_set;
            num_slots = number_of_blocks_in_set;
            for(int i=0; i<number_of_set;i++){
                Set set;
                for(int j=0; j<number_of_blocks_in_set;j++){
                    Slot s;
                    s.tag = 0;
                    s.index =i;
                    s.access_ts =0;
                    s.dirty_bit = false;
                    set.slots.push_back(s);
                }
                sets.push_back(set);
            }
        }
};



#endif