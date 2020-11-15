#pragma once

/*
class BucketData {
     set;
    int is_full;
};
*/

#include <set>
#include <map>
#include "macro.h"

//typedef boost::container::flat_set<int> GrepBucketType;
typedef std::set<int> GrepBucketType;

//typedef std::map<std::string, GrepBucketType> std_map_std_string_std_set_int;
//typedef std::map<int, std::set<std::string>> std_map_int_std_set_std_string;

//BOOST_CLASS_TRACKING(std_map_std_string_std_set_int, boost::serialization::track_never)
//BOOST_CLASS_TRACKING(std_map_int_std_set_std_string, boost::serialization::track_never)

int strends(const char* str, const char* suffix);

class GrepBucket {
    std::map<std::string, int> _bucket_map;

public:
    GrepBucket();
    int get_bucket(std::string& key);
};