#include <stdio.h>

#include "searchtool.h"

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("Usage: searchtool target dir1 dir2 ...");
        exit(1);
    }

    std::string target = argv[1];

    std::vector<std::string> dirs;
    for(int i = 2; i < argc; i ++)
    {
        dirs.push_back(argv[i]);
    }

    GrepIndexer indexer = GrepIndexer();

    indexer.index_files(dirs);

    GrepServer server;

    server.set_words_buckets(indexer._words, indexer._buckets);
    server.exec(target.c_str());

    return 0;
}
