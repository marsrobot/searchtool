# Table of Contents
- [Introduction](#introduction)
- [Run](#Run)
- [Credits](#Credits)

# Introduction

A text search tool, similar to grep, glimpse

# Run
## On Linux:
Step 1: Compile the code by running command

            sh ./run.sh

Step 2: Use the tool by command

            ./searchtool server .

Sample output:
* \*\*\*/searchtool/main.cpp
*      30:     GrepServer server;
*      32:     server.set_words_buckets(indexer._words, indexer._buckets);
*      33:     server.exec(target.c_str());


## On Windows:
* Step 1: Compile the code by open Visual Studio solution searchtool.sln in searchtool-win

* Step 3: Use the tool by running command

            searchtool.exe server .

    Same output will be displayed as the case of Linux.


# Credits
utf8 code is from http://utfcpp.sourceforge.net/
