#pragma once

#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
#include <locale>
#include <algorithm>
#include "macro.h"

int string_to_lower(const char* str, std::string* res);
std::string string_to_lower(std::string s);
int string_is_ascii(const char* str);
int string_contain(const char* str, const char* p_delim);
int string_split_by_many(const char* p_s, std::vector<char> vc, std::vector<std::string>* res);
int string_trim(std::string* p_str, StringPosition pos, std::string* res);

int StringCaseInsensitiveComparison(const std::string& first, const std::string& second);

int string_replace(const char* p_str, char x, char y, std::string* res);

int string_replace_impl(const char* p_str, const char* p_x, const char* p_y, const int replace_all, std::string* res);

int string_replace(const char* string_input, const char* x, const char* y, const int replace_all, const int from_beginning, std::string* res);

int string_begin_with(const char* p_str, const char* p_delim);
