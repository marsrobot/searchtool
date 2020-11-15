#pragma once

#if defined _WIN32 || defined _WIN32_
#include <windows.h>
#endif

#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include "c_function.h"
#include "time_util.h"
#include "string_util.h"


#if defined(OS_UNIX)
int fs_get_directory_base_name(const char* path, std::string* dn, std::string* fn);
int fs_base_name_cc(const char* path, std::string* res);
#endif

std::string fs_extension_name(const char* filename);
std::string fs_read_string(const char* filename);
int fs_is_binary_file(const char* fn);
int fs_file_exist(const char* fn);
int fs_open_file_failure(const char* i_o, const char* stx);
int print_error_number(int num);
int fs_open_file_ifstream(std::ifstream* fsx, const char* fn);
int fs_read_vector_string(const char* fn, std::vector<std::string>* vs);


#if defined(OS_UNIX)
int is_file(const char* fn);
#else
int is_file(const char* fn);
#endif

int fs_access_u(const char* pathname, int mode);
int normalize_path(const char* fn, std::string* res);
int fs_open_directory_failure(const char* stx);
int fs_is_directory(const char* fn);

#if defined(OS_UNIX)
int fs_stat_u(const char* pathname, stat_data* statbuf);
#else
int fs_stat_u(const char* pathname, struct _stat* statbuf);
#endif

#if defined(OS_UNIX)
int is_regular_file(const char* path_name);
int dir_get_ents_impl_unix(const char* dirn, int max_depth, int* p_depth, std::vector<std::string>* p_dirs, std::vector<std::string>* p_files);
#else
int dir_get_ents_impl_win(std::wstring dir, std::list<std::wstring>& dirs, std::list<std::wstring>& files);
#endif

int fs_dir_get_ents_impl(const char* dirn, int max_depth, std::vector<std::string>* p_dirs, std::vector<std::string>* p_files);
int fs_dir_get_ents(const char* dirn, int max_depth, std::vector<std::string>* dirs, std::vector<std::string>* files);

#if defined(OS_UNIX)
int fs_time_modified_impl(const char* fn, UnixDateTime* res);
UnixDateTime fs_time_modified(const char* fn);

#else
UnixDateTime fs_time_modified(const char* fn);
#endif

std::string fs_time_modified_as_string(const char* fn);
