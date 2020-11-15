#include <cstring>
#include <string>
#include "macro.h"

CodeLocation::CodeLocation(const char* file_i, int64_t line_i, const char* function_i)
{
    if (function_i == nullptr)
        this->_function = __func__;
    else if (strlen(function_i) == 0)
        this->_function = __func__;
    else {
        // fprintf(stderr, "function=%p, strlen(function)=%ld\n", function_i, int64_t(strlen(function_i)));
        this->_function = std::string(function_i);
    }

    this->_file = file_i;
    this->_line = line_i;

    this->_date = CodeLocation_date;
    this->_time = CodeLocation_time;
}

std::string CodeLocation::filename()
{
    return this->_file;
}

int64_t CodeLocation::line()
{
    return this->_line;
}

std::string CodeLocation::function()
{
    return this->_function;
}

std::string CodeLocation::to_text()
{
    return this->_file + ":" + std::to_string(this->_line) + " " + this->_function;
}
