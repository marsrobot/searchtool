#include "GrepIndexer.h"


int GrepIndexer::index_files(std::vector<std::string> args)
{
	std::vector<std::string> dirs;

	for (int64_t i = 0; i < int64_t(args.size()); i++) {
		std::string input_path = args[i];
		std::string normalized_path;
		normalize_path(input_path.c_str(), &normalized_path);

		fprintf(stderr, "Indexing %s\n", normalized_path.c_str());
		dirs.push_back(normalized_path);
	}

	if (dirs.size() > 0) {
	}
	else {
		exit(1);
	}

	std::vector<std::string> ent_dirs;
	std::vector<std::string> ent_files;

	GrepBucket bucket;
	for (auto& dir : dirs) {
		if (!fs_is_directory(dir.c_str()))
			continue;
		fs_dir_get_ents(dir.c_str(), 99, &ent_dirs, &ent_files);

		std::sort(ent_files.begin(), ent_files.end(), [](const std::string& x, const std::string y) {
			UnixDateTime xdt = fs_time_modified(x.c_str());
			UnixDateTime ydt = fs_time_modified(y.c_str());

			time_t x_t = unix_datetime_get_data(&xdt);
			time_t y_t = unix_datetime_get_data(&ydt);
			return x_t < y_t;
			});
		std::sort(ent_files.begin(), ent_files.end());

		/*
		std::vector<std::string> exts{ ".c", ".cc", ".cpp", ".cxx", ".hpp", ".h",
		   ".cs", ".java",
		   ".go",
		   ".py",
		   ".js", ".javascript", "jsx", ".ts",
		   ".f", ".f77", ".f90", ".for", ".fortran", ".f95",
		 };
		 */

		int64_t finished = 0;
		int64_t n_threads = 16;

		do {
			std::vector<std::thread> threads(n_threads);
			std::vector<GrepBatch> batches(n_threads);
			std::vector<std::vector<std::string> > vvs(n_threads);
			int64_t batch_size = std::max<int64_t>(1, std::min<int64_t>(int64_t(std::ceil(double(ent_files.size()) / n_threads / 10)), 2048));

			int64_t i_max = 0;
			for (int64_t i = 0; i < n_threads; i++) {
				int64_t begin = finished;
				int64_t end = std::min<int64_t>(finished + batch_size, ent_files.size());
				if (begin < end) {
					vvs[i] = std::vector<std::string>(ent_files.begin() + begin, ent_files.begin() + end);
					finished += end - begin;
					i_max = i;
				}
			}

			for (int64_t i = 0; i < i_max; ++i) {
				threads[i] = std::thread(grep_index_batch_run, std::ref(vvs[i]), std::ref(batches[i]));
			}
			for (int64_t i = 0; i < i_max; ++i) {
				threads[i].join(); //Join the threads with the main thread
			}

			for (int64_t i = 0; i < i_max; ++i) {
				std::map<std::string, GrepBucketType>& batch_words(batches[i].get_words());
				for (const std::pair<std::string, GrepBucketType>& kv : batch_words) {
					if (_words.find(kv.first) == _words.end()) {
						_words[kv.first] = GrepBucketType();
					}
					for (auto& x : kv.second) {
						_words[kv.first].insert(x);
					}
				}

				std::map<int, std::set<std::string> >& batch_buckets(batches[i].get_buckets());
				for (const std::pair<int, std::set<std::string> >& kv : batch_buckets) {
					if (_buckets.find(kv.first) == _buckets.end()) {
						_buckets[kv.first] = std::set<std::string>();
					}
					for (auto& x : kv.second) {
						_buckets[kv.first].insert(x);
					}
				}
			}
		} while (finished < int64_t(ent_files.size()));
	}
	return 0;
}

int GrepIndexer::save_index(std::vector<std::string> args)
{
	return 0;
}

int GrepIndexer::run(std::vector<std::string> args)
{
	this->index_files(args);
	this->save_index(args);
	return 0;
}

int GrepIndexer::print()
{
	fprintf(stdout, "words:\n");
	for (auto it = this->_words.begin(); it != this->_words.end(); ++it) {
        fprintf(stdout, "%s: ", it->first.c_str());
		for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
			fprintf(stdout, "%d ", int(*jt));
		}
		fprintf(stdout, "\n");
	}

	fprintf(stdout, "buckets:\n");
	for (auto it = this->_buckets.begin(); it != this->_buckets.end(); ++it) {
		fprintf(stdout, "%d: ", it->first);
		for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
			fprintf(stdout, "%s ", jt->c_str());
		}
		fprintf(stdout, "\n");
	}

	return 0;
}


void grep_index_batch_run(std::vector<std::string>& vec, GrepBatch& batch)
{
	batch.run(vec);
}
