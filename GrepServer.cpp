
#include <map>
#include <boost/lockfree/queue.hpp>
#include "GrepBucket.h"
#include "GrepServer.h"

void GrepServer::load_from_file()
{
	_keys.reserve(_words.size());
	for (auto& kv : _words)
		_keys.push_back(kv.first);
}

void GrepServer::set_words_buckets(std::map<std::string, GrepBucketType> new_words,
	std::map<int, std::set<std::string> > new_buckets)
{
	this->_words = new_words;
	this->_buckets = new_buckets;

	_keys.reserve(_words.size());
	for (auto& kv : _words)
		_keys.push_back(kv.first);
}
GrepServer::GrepServer()
{
	int64_t pid = 0;
	_words = std::map<std::string, GrepBucketType>();
	_buckets = std::map<int, std::set<std::string> >();
}

int GrepServer::exec(std::string needle)
{
	std::set<std::string> matches;

	boost::lockfree::queue<int64_t> queue(1024);
	assert(queue.is_lock_free());
	const int64_t N = _keys.size();

#if defined(USE_OPENMP)
#if !defined(__MINGW32__)
	omp_set_dynamic(0); // Explicitly disable dynamic teams
	omp_set_num_threads(8); // Use 4 threads for all consecutive parallel regions
#pragma omp parallel
#pragma omp for
#endif
#endif
	for (int64_t i = 0; i < N; i++) {
		if (string_contain(_keys[i].c_str(), needle.c_str()))
			queue.push(i);
	}

	int64_t key_index;
	while (queue.pop(key_index)) {
		matches.insert(_keys[key_index]);
	}

	std::vector<std::string> vec_matches(matches.begin(), matches.end());

	std::set<std::pair<std::string, std::string> > already_got;

	for (auto& match : matches) {
		for (int index : _words[match]) {
			for (const std::string& filename : _buckets[index]) {
				std::vector<std::string> lines;
				try
				{
					fs_read_vector_string(filename.c_str(), &lines);
				}
				catch (std::runtime_error const& ex)
				{
					fprintf(stderr, "%s\n", ex.what());
					continue;
				}
				int first_found_line = true;
				for (int64_t j = 0; j < int64_t(lines.size()); j++) {
					std::pair<std::string, std::string> key{ filename, lines[j] };

					if (string_contain(lines[j].c_str(), needle.c_str())
						&& already_got.find(key) == already_got.end()) {
						already_got.insert(key);
						if (first_found_line) {
							fprintf(stdout, "%s\n", filename.c_str());
							first_found_line = false;
						}
						fprintf(stdout, "    %4ld: %s\n", j, string_colorize(lines[j], needle).c_str());
					}
				}
			}
		}
	}

	return 0;
}