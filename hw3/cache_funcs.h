#include "Cache.h"
#include <string>

#ifndef CACHE_FUNCS_H
#define CACHE_FUNCS_H


std::string convertToString(char * char_array, int size);

std::vector<std::string> to_Vector(std::string s);

//a store writes to the cache as well as to memory
int write_through(Cache ca, Slot * s);

//a cache miss during a store does not modify the cache
int no_write_allocate(Cache ca, Slot * s);








#endif