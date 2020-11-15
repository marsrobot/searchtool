#pragma once

#include "regex_util.h"

class GrepServer {
    std::map<std::string, GrepBucketType> _words; // save
    std::map<int, std::set<std::string> > _buckets; // save
    std::vector<std::string> _keys;

public:
    void load_from_file();
    void set_words_buckets(std::map<std::string, GrepBucketType> new_words,
        std::map<int, std::set<std::string> > new_buckets);
    
    GrepServer();
    int exec(std::string needle);
};
