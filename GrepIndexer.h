#pragma once

#include <thread>
#include "macro.h"
#include "SimpleLogger.h"

#include "GrepBatch.h"
#include "GrepBucket.h"

void grep_index_batch_run(std::vector<std::string>& vec, GrepBatch& batch);

class GrepIndexer {
public:
	std::map<std::string, GrepBucketType> _words;
	std::map<int, std::set<std::string> > _buckets;

public:
	int index_files(std::vector<std::string> args);
	int save_index(std::vector<std::string> args);
	int print();
	int run(std::vector<std::string> args);
};
