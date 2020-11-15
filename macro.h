#pragma once

#if defined _WIN32 || defined _WIN32_
#include "win_header.h"
#endif

#include <string>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#define OS_WIN 1
#endif

#ifndef OS_WIN
#ifndef __unix__
#define __unix__
#endif
#endif

#if defined(__unix__) || defined(__linux__)
#define OS_UNIX 1
#endif

#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
#define OS_64BIT
#else
#define OS_32BIT
#endif

#if defined(OS_UNIX)
#define COLOR_RED "\033[22;31m"
#define COLOR_GREEN "\033[22;32m"
#define COLOR_YELLOW "\033[22;33m"
#define COLOR_BLUE "\033[22;34m"
#define COLOR_MAGENTA "\033[22;35m"
#define COLOR_CYAN "\033[22;36m"
//#define COLOR_CYAN      "\e[1;36m" // bright cyan
#define COLOR_WHITE "\e[1;37m"
#define COLOR_DEFAULT "\033[0;22m"
#define COLOR_FUNCTION COLOR_CYAN
#define COLOR_FILE COLOR_CYAN
#define COLOR_POINTER COLOR_RED
#define COLOR_DIRECTORY COLOR_BLUE
#define COLOR_VARIABLE COLOR_RED
#define COLOR_VALUE COLOR_GREEN
#define COLOR_ERROR COLOR_RED
#define COLOR_INFORMATION COLOR_YELLOW
#else
#define COLOR_RED ""
#define COLOR_GREEN ""
#define COLOR_YELLOW ""
#define COLOR_BLUE ""
#define COLOR_MAGENTA ""
#define COLOR_CYAN ""
#define COLOR_DEFAULT ""
#define COLOR_FUNCTION ""
#define COLOR_FILE ""
#define COLOR_POINTER ""
#define COLOR_DIRECTORY ""
#define COLOR_VARIABLE ""
#define COLOR_VALUE ""
#define COLOR_ERROR ""
#define COLOR_INFORMATION ""
#endif

#define PATH_MAX 4192

extern "C" {
    enum StringPosition {
        StringPositionFront = 0,
        StringPositionBack = 2,
        StringPositionBoth = 3
    };

    // datetime
    struct UnixDateTime {
        time_t _time;
    };

}

#if defined(OS_WIN)
typedef struct _stat stat_data;
typedef std::wstring os_string;
//#   define stat_data struct _stat;
typedef wchar_t OS_CHAR;
typedef wchar_t os_char;
#else
typedef struct stat stat_data;
//#   define stat_data struct stat;
typedef std::string os_string;
typedef char OS_CHAR;
typedef char os_char;
#endif

#if defined(OS_WIN)
typedef struct _stat stat_data;
typedef std::wstring os_string;
//#   define stat_data struct _stat;
typedef wchar_t OS_CHAR;
typedef wchar_t os_char;
#else

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct stat stat_data;
//#   define stat_data struct stat;
typedef std::string os_string;
typedef char OS_CHAR;
typedef char os_char;
#endif



static std::string CodeLocation_date = __DATE__;
static std::string CodeLocation_time = __TIME__;

class CodeLocation {
public:
    std::string _file;
    int64_t _line;
    std::string _function;
    std::string _date;
    std::string _time;

    CodeLocation(const char* file_i, int64_t line_i, const char* function_i);
    std::string filename();
    int64_t line();
    std::string function();
    virtual std::string to_text();
};

#define CODE_LOCATION ::CodeLocation(std::string(__FILE__).c_str(), __LINE__, std::string(__func__).c_str())
