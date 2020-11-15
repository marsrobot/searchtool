#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include <unordered_set>
#include "hash_util.h"

size_t universal_hash_value(const char* data)
{
    std::hash<std::string> hasher;
    return hasher(std::string(data));
}
