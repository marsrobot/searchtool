#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "utf8.h"

#include "macro.h"


int is_regular_file(const char* path_name);
int normalize_path(const char* fn, std::string* res);
// int realpath_boost(const char* path, std::string* res);
int string_begin_with(const char* str, const char* delim);
int string_case_insensitive_replace(const char* input, const char* a, const char* b, std::string* res);
int string_contain(const char* str, const char* delim);
int string_end_with(const char* s_input, const char* suffix_input);
int string_printf(std::string* res, const char* format, ...);
int string_replace_impl(const char* string_input, const char* x, const char* y, const int replace_all, std::string* res);
int string_replace(const char* string_input, const char* x, const char* y, int replace_all, int from_beginning, std::string* res);
int string_reverse_trim(char* text, int64_t* len, std::string* res); // remove trailing spaces
int string_split_by_many(const char* s, std::vector<char> vc, std::vector<std::string>* res);
int string_split_impl(const char* str, const char* delim, std::vector<std::string>* res);
int string_split_two_delim(const char* string_input, char delim_0, char delim_1, std::vector<std::string>* res);
int string_split_with_brackets(const char* s, const char* delim, std::vector<char> brackets, int nparts, std::vector<std::string>* res);
int string_trim(std::string* str, StringPosition pos);
int unicode_utf8_to_utf16(const char* content, std::wstring* res);
int unicode_utf16_to_utf8(wchar_t const* p_wcontent, std::string* res);
int fprintf(FILE* fout, const char* format, ...);
void perror_exit(const char* s);
void perror(const char* s);
void* malloc_check(int size);




// file system
#if defined _WIN32 || defined _WIN32_ || defined _EMX_ || defined _DJGPP_
// Win32, OS/2, DOS
#define HAS_DEVICE(P)                                                       \
    ((((P)[0] >= 'A' && (P)[0] <= 'Z') || ((P)[0] >= 'a' && (P)[0] <= 'z')) \
        && (P)[1] == ':')
#define FILESYSTEM_PREFIX_LEN(P) (HAS_DEVICE(P) ? 2 : 0)
#define IS_SLASH(C) ((C) == '/' || (C) == '\\')
#endif

#ifndef FILESYSTEM_PREFIX_LEN
#define FILESYSTEM_PREFIX_LEN(Filename) 0
#endif

#ifndef IS_SLASH
#define IS_SLASH(C) ((C) == '/')
#endif

extern int flcreate_locked_file;



#define PATH_MAX_GUESS 1024


#define CODE_LOCATION ::CodeLocation(std::string(__FILE__).c_str(), __LINE__, std::string(__func__).c_str())



/*
void error(const char*fmt, ... )
{
	va_list ap;
	char buf[1000];

	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	fprintf(stdout, "error: %s\n", buf);
	assert(1 == 0);
}
*/








// You could also take an existing vector as a parameter.
int string_split(const char* str, char delimiter, std::vector<std::string>* res);






















