#include "Cache.h"
#include <string>

#ifndef CACHE_FUNCS_H
#define CACHE_FUNCS_H


//a store writes to the cache as well as to memory
void write_through(Cache &ca, Slot * s, int timestamp, bool dirty);

//slot does exist by searching
Slot * slotExists(Cache& ca, Slot *s);

//determines whwat to call then whether hit or miss
// void hitOrMiss(Cache& ca, Slot * s );
//checks if cache is full
bool isCacheFull(Cache& ca, Slot * s);

bool isCacheEmpty(Cache& ca, Slot * s);

void evictionFunction(Cache& ca, Slot * s, int timestamp, int byte_size, bool lru_or_fifo);

void update_load_ts(Cache& ca, Slot * s);

#endif