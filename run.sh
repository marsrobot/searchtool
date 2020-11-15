g++ -D_DEBUG -D_GLIBCXX_USE_CXX11_ABI=1 -fmax-errors=3 -D_UNICODE -DUNICODE -w -fPIC -openmp -fopenmp -std=c++11 -rdynamic -O \
    c_function.cpp \
    CodeLocation.cpp \
    env_util.cpp \
    fs_util.cpp \
    GrepBatch.cpp \
    GrepBucket.cpp \
    GrepIndexer.cpp \
    GrepServer.cpp \
    GrepTest.cpp \
    hash_util.cpp \
    main.cpp \
    regex_util.cpp \
    SimpleLogger.cpp \
    string_util.cpp \
    time_util.cpp \
-o searchtool -Wall -I/srv/dev/cc -lpcre -lboost_serialization -lboost_system