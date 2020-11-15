#pragma once

#include <map>
#include <vector>
#include <set>

#include "macro.h"
#include "string_util.h"
#include "fs_util.h"
#include "GrepBucket.h"

class GrepBatch {
    std::map<std::string, GrepBucketType> _words; // save
    std::map<int, std::set<std::string> > _buckets; // save
    int is_text_file(std::string filename);

public:
    std::map<std::string, GrepBucketType>& get_words();
    std::map<int, std::set<std::string> >& get_buckets();
    int run(std::vector<std::string> args);

};
