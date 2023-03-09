#include "Cache.h"
#include <string>

#ifndef CACHE_FUNCS_H
#define CACHE_FUNCS_H


std::string convertToString(char * char_array, int size);

std::vector<std::string> to_Vector(std::string s);

//a store writes to the cache as well as to memory
void write_through(Cache& ca, Slot * s);

//a cache miss during a store does not modify the cache
void no_write_allocate(Cache ca, Slot * s);
//updates the cache only, not the memory, so the memory counter is not incremented. The dirty bit is changed to true
//in case of eviction
void write_back(Cache ca, Slot * s);
//increment the memory counter and write to cache as well
void write_allocate(Cache ca, Slot * s);
//slot does exist by searching
bool slotExists(Cache& ca, Slot *s);

//determines whwat to call then whether hit or miss
// void hitOrMiss(Cache& ca, Slot * s );
//checks if cache is full
bool isCacheFull(Cache& ca, Slot * s);

void evictionFunction(Cache& ca, Slot * s);

//returning simulator results
int& return_total_Loads();
int& return_total_Stores();
int& return_load_Misses();
int& return_load_Hits();
int& return_store_Hits();
int& return_store_Misses();
int& return_total_cycles();

#endif