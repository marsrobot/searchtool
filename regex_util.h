#pragma once

#include <algorithm>
#include <string>
#include <set>

#include "macro.h"
#include "fs_util.h"
#include "SimpleLogger.h"

#include "pcre.h"

std::vector<std::vector<std::string> > regex_match(std::string& regex_i, std::string string_i, int case_sensitive);

std::string string_colorize(std::string& s, std::string& regex_str);

