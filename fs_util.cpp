#if defined(_WIN32) || defined(_WIN32_)
#include <windows.h>
#else
#include <libgen.h>
#include <dirent.h>
#endif

#include <cstring>

#include "macro.h"
#include "fs_util.h"

#if defined(OS_UNIX)
int fs_get_directory_base_name(const char* path, std::string* dn, std::string* fn)
{
    char* p2[2];
    for (int64_t i = 0; i < 2; i++)
        p2[i] = new char[strlen(path) + 1];
    strcpy(p2[0], path);
    strcpy(p2[1], path);
    *dn = std::string(::dirname(p2[0]));
    *fn = std::string(::basename(p2[1]));
    for (int64_t i = 0; i < 2; i++)
        delete[] p2[i];
    /*  bug
            ch  p2[ path.size() + 1 ];
            strcpy( p2, path );
            dn = std::string( dirname ( p2 ) );
            fn = std::string( basename( p2 ) );
            */
    return 0;
}

int fs_base_name_cc(const char* path, std::string* res)
{
    std::string dn;
    std::string fn;
    fs_get_directory_base_name(path, &dn, &fn);
    *res = fn;
    return 0;
}
#endif

std::string fs_extension_name(const char* filename)
{
    char* p = new char[strlen(filename) + 1];
    strcpy(p, filename);
    std::string base;
#if defined(OS_UNIX)
    fs_base_name_cc(p, &base);
#else
    base = p;
#endif
    const char* fn = base.c_str();

    if (strlen(fn) == 0) {
        delete[] p;
        return ("");
    }
    //if(filename[0] == '.')
    //return "";

    const char* dot = strrchr(fn, '.');
    if (!dot) {
        delete[] p;
        return ("");
    }

    //if(!dot || dot == fn)
    //return "";
    return dot + 1;
}

std::string fs_read_string(const char* filename)
{
    char* buffer = 0;
    int64_t length = 0;
    FILE* f = fopen(filename, "rb");

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = (char*)malloc(length);
        if (buffer) {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }

    //std::vector<char> vec = fs_read_string(fn);
    std::string s(buffer, buffer + length);
    return s;
}

int fs_is_binary_file(const char* fn)
{
    std::string data = fs_read_string(fn);
    int is_binary = memchr((char*)(&(data[0])), ('\0'), data.size()) != nullptr;
    return is_binary;
}

int fs_file_exist(const char* fn)
{
    std::ifstream ifile(fn);
    return ifile.good();
}

int fs_open_file_failure(const char* i_o, const char* stx)
{
    fprintf(stderr, "%sError: cannot open %s%s file %s%s", COLOR_ERROR,
        i_o, COLOR_FILE, stx, COLOR_DEFAULT);
    return 0;
}

int print_error_number(int num)
{
    int _data = num;
    int lErrCode;
    char* lErrString;

    lErrCode = strtoul(std::to_string(_data).c_str(), nullptr, 0);
    lErrString = strerror(lErrCode);
    if (lErrString == nullptr) {
        strcpy(lErrString, "Unknown");
    }

    fprintf(stdout, "errno = \r\n");
    ;
    //#define  BEAUTIFY__MANY_ERRN
#ifdef BEAUTIFY__MANY_ERRN
    for (int j = 0; j < 3 - std::to_string(i).length(); ++j)
        printf(" ");
#endif
    fprintf(stdout, "%ld - %s\r\n", lErrCode, lErrString);
    return 0;
}

int fs_open_file_ifstream(std::ifstream* fsx, const char* fn)
{
    //int lErrCode;
    //char* lErrString;
    fsx->open(fn);
    if (fsx) {
        return true;
        //fs_open_file_success( "input", fn );
    }
    else {
        //switch (errno)
        //{
        //case EACCES:  std::cout<<COLOR_ERROR<< "Permission denied.\n";        break;
        //case EINVACC: std::cout<<COLOR_ERROR<< "Invalid access mode.\n";      break;
        //case EMFILE:  std::cout<<COLOR_ERROR<< "No file handle available.\n"; break;
        //case ENENT:   std::cout<<COLOR_ERROR<< "File or path not found.\n";   break;
        //default:      std::cout<<COLOR_ERROR<< "Unknown error.\n";            break;
        //}
        fs_open_file_failure("input", fn);
        print_error_number(errno);
        return false;
    }
}

int fs_read_vector_string(const char* fn, std::vector<std::string>* vs)
{
    //std::vector<std::string> vs;
    std::ifstream file;
    if (!fs_file_exist(fn)) {
        fprintf(stdout, "Error: file '%s' does not exist\n", fn);
    }

    vs->clear();
    int flag = fs_open_file_ifstream(&file, fn);
    if (flag) {
        std::string str;
        while (getline(file, str))
            vs->push_back(str); // ignore comment line
        file.close();
    }
    return 0;
}

#if defined(OS_UNIX)
int is_file(const char* fn)
{
    stat_data path_stat;
    stat(fn, &path_stat);
    return S_ISREG(path_stat.st_mode);
}
#else
int is_file(const char* fn)
{
    std::ifstream ifile(fn);
    return ifile.good();
}
#endif

int fs_access_u(const char* pathname, int mode)
{
#if defined(OS_UNIX)
    return access(pathname, mode);
#else
    std::wstring wpathname;
    unicode_utf8_to_utf16(pathname, &wpathname);
    return _waccess(wpathname.c_str(), mode);
#endif
}

int normalize_path(const char* fn, std::string* res)
{
    std::string new_fn = fn;
    while (new_fn.find(("//")) != std::string::npos) {
        std::string old_val = std::string("//");
        std::string new_val = std::string("/");
        string_replace(fn, old_val.c_str(), new_val.c_str(), true, true, &new_fn);
    }
    *res = new_fn;
    return 0;
}

int fs_open_directory_failure(const char* stx)
{
    fprintf(stderr, "%sError: cannot open directory %s%s%s\n", COLOR_ERROR,
        COLOR_DIRECTORY, stx, COLOR_DEFAULT);
    // Error(-1).error("crash");
    return 0;
}

int fs_is_directory(const char* fn)
{
    return !is_file(fn);
}

#if defined(OS_UNIX)
int fs_stat_u(const char* pathname, stat_data* statbuf)
{
    return stat(pathname, statbuf);
}
#else
int fs_stat_u(const char* pathname, struct _stat* statbuf)
{
    return _stat(pathname, statbuf);
}
#endif

#if defined(OS_UNIX)
int is_regular_file(const char* path_name)
{
#if defined(OS_UNIX)
    stat_data path_stat;
    fs_stat_u(path_name, &path_stat);
    mode_t mode = path_stat.st_mode;
    switch (mode & S_IFMT) {
    case S_IFREG:
        return true;
    case S_IFDIR:
        return false;
    case S_IFLNK:
        return false;
    case S_IFCHR:
        return false;
    case S_IFBLK:
        return false;
#if defined(S_IFFIFO)
    case S_IFFIFO:
        return false;
#endif
#if !defined(BEOS) && defined(S_IFSOCK)
    case S_IFSOCK:
        return false;
#endif
    default:
        return false;
    }
#else
    //struct _stat path_stat;
    //_stat(path_name, &path_stat);
    boost::filesystem::path p(path_name);
    return boost::filesystem::is_regular_file(p);
#endif
}

int dir_get_ents_impl_unix(const char* dirn, int max_depth, int* p_depth, std::vector<std::string>* p_dirs, std::vector<std::string>* p_files)
{
    (*p_depth)++;
    std::vector<std::string> lfi;
    std::vector<std::string> lfo;
    DIR* dirp = opendir(dirn);
    if (dirp) {
        //fs_open_directory_success( dirn );
        struct dirent* dp = nullptr;

        char actualpath[PATH_MAX + 1];
        char* ptr;
        ptr = ::realpath(dirn, actualpath);

        while ((dp = readdir(dirp)) != nullptr) {
            std::string filename(dp->d_name);
            if (filename == "." || filename == "..") // skip these
                continue;

            string_printf(&filename, "%s/%s", ptr, filename.c_str());
            normalize_path(filename.c_str(), &filename);
            if (dp->d_type & DT_DIR) {
                lfo.push_back(filename);
            }
            else {
                if (is_regular_file(filename.c_str()))
                    lfi.push_back(filename);
                // we only care regular file. FIFO files hangs file reading. -marsrobot 12/05/2018
                // regular file found
                //std::cout<< "filename is: " << filename << std::endl;
            }
        }
        closedir(dirp);
    }
    else {
        print_error_number(errno);
        fs_open_directory_failure(dirn);
        //std::cout<<COLOR_ERROR<< "Exit"<<std::endl;
        //exit( EXIT_FAILURE );
        return 0;
    }

    for (auto& filename : lfi)
        p_files->push_back(filename);
    for (auto& dirname : lfo)
        p_dirs->push_back(dirname);

    if (*p_depth < max_depth)
        for (auto& dirname : lfo) {
            if (!string_contain(dirname.c_str(), "/mnt/hgfs")
                && !string_begin_with(dirname.c_str(), "/proc")
                && !string_begin_with(dirname.c_str(), "/dev")
                && !string_begin_with(dirname.c_str(), "/lost+found")
                && !string_begin_with(dirname.c_str(), "/run"))
                dir_get_ents_impl_unix(dirname.c_str(), max_depth, p_depth, p_dirs, p_files);
        }

    (*p_depth)--;
    return 0;
}
#else
int dir_get_ents_impl_win(std::wstring dir, std::list<std::wstring>& dirs, std::list<std::wstring>& files)
{
    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    TCHAR szDir[MAX_PATH];
    size_t length_of_arg;
    DWORD dwError = 0;

    // Check that the input path plus 3 is not longer than MAX_PATH.
    // Three characters are for the "\*" plus nullptr appended below.
    StringCchLength(dir.c_str(), MAX_PATH, &length_of_arg);
    if (length_of_arg > (MAX_PATH - 3))
    {
        wprintf(TEXT("\nDirectory path is too long.\n"));
        return (-1);
    }

    // Prepare string for use with FindFile functions.  First, copy the
    // string to a buffer, then append '\*' to the directory name.
    StringCchCopy(szDir, MAX_PATH, dir.c_str());
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

    // Find the first file in the directory.
    HANDLE hFind = INVALID_HANDLE_VALUE;
    hFind = FindFirstFile(szDir, &ffd);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        wprintf(TEXT("FindFirstFile"));
        return dwError;
    }

    // List all the files in the directory with some info about them.
    do
    {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            //if (!(strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "..") == 0))
            if (!(wcscmp(ffd.cFileName, TEXT(".")) == 0 || wcscmp(ffd.cFileName, TEXT("..")) == 0))
            {
                //_ftprintf(stderr, TEXT("%s\n"), ffd.cFileName);
                std::wstring s = dir + TEXT("\\") + ffd.cFileName;
                dirs.push_back(s);
                //_ftprintf(stderr, TEXT("%s\n"), s.c_str());
                SetCurrentDirectory(s.c_str());
                dir_get_ents_impl_win(s.c_str(), dirs, files);
                SetCurrentDirectory(dir.c_str());
            }
        }
        else
        {
            filesize.LowPart = ffd.nFileSizeLow;
            filesize.HighPart = ffd.nFileSizeHigh;
            //_ftprintf(stderr, TEXT("%s\n"), ffd.cFileName);
            files.push_back(dir + TEXT("\\") + ffd.cFileName);
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        wprintf(TEXT("FindFirstFile"));
    }

    FindClose(hFind);
    return dwError;
}
#endif

int fs_dir_get_ents_impl(const char* dirn, int max_depth, std::vector<std::string>* p_dirs, std::vector<std::string>* p_files)
{
#if defined(OS_UNIX)
    std::vector<std::string> dirs;
    std::vector<std::string> files;
    int depth = 0;
    dir_get_ents_impl_unix(dirn, max_depth, &depth, &dirs, &files);
#else

    int depth = 0;
    std::list<std::wstring> wdirs;
    std::list<std::wstring> wfiles;

    std::wstring wdirn;
    unicode_utf8_to_utf16(dirn, &wdirn);

    dir_get_ents_impl_win(wdirn, wdirs, wfiles);

    std::vector<std::wstring> vec_wdirs(wdirs.begin(), wdirs.end());
    std::vector<std::wstring> vec_wfiles(wfiles.begin(), wfiles.end());

    std::vector<std::string> dirs;
    std::vector<std::string> files;

    int64_t n;
    n = vec_wdirs.size();
    for (int64_t i = 0; i < n; i++)
    {
        std::string dir_str;
        unicode_utf16_to_utf8(vec_wdirs[i].c_str(), &dir_str);
        dirs.push_back(dir_str);
    }
    n = vec_wfiles.size();
    for (int64_t i = 0; i < n; i++)
    {
        std::string file_str;
        unicode_utf16_to_utf8(vec_wfiles[i].c_str(), &file_str);
        files.push_back(file_str);
    }
#endif

    std::sort(files.begin(), files.begin(), StringCaseInsensitiveComparison);
    std::sort(dirs.begin(), dirs.end(), StringCaseInsensitiveComparison);

    std::vector<std::string> files_vector;
    int64_t n_files = files.size();
    for (int64_t i = 0; i < n_files; i ++)
    {
        std::string file = files[i];
        bool flag;
#if defined(OS_UNIX)
        flag = (fs_access_u(file.c_str(), F_OK) != -1);
#else
        flag = (fs_access_u(file.c_str(), 0) != -1);
#endif
        if (flag)
            files_vector.push_back(file);
        else {
            // these are symbolic links
            fprintf(stdout, "Failed to get file '%s'\n", file.c_str());
        }
    }
    *p_files = files_vector;
    std::vector<std::string> dirs_vector;
    for (auto& x : dirs) {
#if defined(OS_UNIX)
        if (fs_access_u(x.c_str(), F_OK) != -1)
#else
        if (fs_access_u(x.c_str(), 0) != -1)
#endif
            dirs_vector.push_back(x);
        else
            fprintf(stdout, "Failed to get file '%s'\n", x.c_str());
    }
    *p_dirs = dirs_vector;
    return 0;
}

int fs_dir_get_ents(const char* dirn, int max_depth, std::vector<std::string>* dirs, std::vector<std::string>* files)
{
    return fs_dir_get_ents_impl(dirn, max_depth, dirs, files);
}

#if defined(OS_UNIX)
int fs_time_modified_impl(const char* fn, UnixDateTime* res)
{
#if defined(_WIN32)
    struct _stat buf;
#else
    stat_data buf;
#endif

    fs_stat_u(fn, &buf);
    int result = fs_stat_u(fn, &buf);
#if defined(OS_UNIX)
    if (result != 0) {
        perror("Problem getting information");
        switch (errno) {
        case ENOENT:
            fprintf(stdout, "File %s not found.\n", fn);
            break;
        case EINVAL:
            fprintf(stdout, "Invalid parameter to fs_stat_u.\n");
            break;
        default:
            // Should never be reached.
            fprintf(stdout, "Unexpected error in fs_stat_u.\n");
        }
    }
    else {
    }
#if defined(__linux__)
    unix_datetime_from_time_t(res, &(buf.st_mtim.tv_sec));
    return 0;
#elif defined(__unix__)
    unix_datetime_from_time_t(res, &buf.st_mtimespec.tv_sec);
    return 0;
#endif
#else
    unix_datetime_from_time_t(res, &buf.st_mtime);
    return 0;
#endif
}

UnixDateTime fs_time_modified(const char* fn)
{
    UnixDateTime res;
    fs_time_modified_impl(fn, &res);
    return res;
}

#else
UnixDateTime fs_time_modified(const char* fn)
{
    struct _stat buf;
    fs_stat_u(fn, &buf);
    int result = fs_stat_u(fn, &buf);
    UnixDateTime u_dt;
    u_dt._time = buf.st_mtime;
    return u_dt;
}
#endif


std::string fs_time_modified_as_string(const char* fn)
{
    UnixDateTime udt;
    time_t t;
    udt = fs_time_modified(fn);

    unix_datetime_get_data(&udt, &t);

    struct tm* timepointer = localtime(&t);
    static char result[20];
    std::sprintf(result, ("%d/%02d/%02d %.2d:%.2d:%.2d"),
                 1900 + timepointer->tm_year,
                 timepointer->tm_mon + 1,
                 timepointer->tm_mday,
                 timepointer->tm_hour,
                 timepointer->tm_min,
                 timepointer->tm_sec);
    return result;
}









