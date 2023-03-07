#include <stdio.h>
#include <vector>
#include <stdint.h>
#ifndef CACHE_H
#define CACHE_H
   struct Slot {
        uint32_t tag;
        uint32_t index;
        uint32_t offset;
        uint32_t load_ts,
        access_ts;
        };
    struct Set {
        std::vector<Slot> slots;
        };
class Cache{
 
  public:
     std::vector<Set> sets;
        Cache(int number_of_set, int number_of_blocks_in_set, int number_of_bytes_per_memory){
            for(int i=0; i<number_of_set;i++){
                Set set;
                for(int i=0; i<number_of_blocks_in_set;i++){
                    Slot s;
                    s.tag = 0;
                    s.index =i;
                    s.offset = 0;
                    s.load_ts = 0;
                    s.access_ts =0;
                    set.slots.push_back(s);
                }
                sets.push_back(set);
            }
        }
};



#endif