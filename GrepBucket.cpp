#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include "GrepBucket.h"
#include "hash_util.h"

int strends(const char* str, const char* suffix)
{
	if (!str || !suffix)
		return 0;
	uint64_t lenstr = strlen(str);
	uint64_t lensuffix = strlen(suffix);
	if (lensuffix > lenstr)
		return 0;
	return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

GrepBucket::GrepBucket()
{
}

int GrepBucket::get_bucket(std::string& key)
{
	if (this->_bucket_map.find(key) != this->_bucket_map.end()) {
		return this->_bucket_map[key];
	}
	else {
		uint64_t hash = universal_hash_value(key.c_str());
		int bucket = hash % (256 * 1024);
		this->_bucket_map[key] = bucket;
		return bucket;
	}
}
