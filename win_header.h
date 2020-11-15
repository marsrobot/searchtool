#pragma once

#define _SCL_SECURE_NO_WARNINGS

#if defined(_WIN32)
// #include <winsock2.h>
// #include <Winsock.h>
#include <windows.h>
// #include <ws2tcpip.h>
// #define close closesocket
#else
#define UNICODE
#define _UNICODE
#endif

#if defined(OS_WIN)

#define _SCL_SECURE_NO_WARNINGS

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#endif


#if defined(UNICODE)
#define __TEXT(quote) L##quote // r_winnt
#define TEXT(quote) __TEXT(quote) // r_winnt
#else
#ifndef TEXT
#define TEXT(quote) quote
typedef char TCHAR;
typedef char tchar;
typedef std::char tchar;
#endif
#endif

#include <string>
#include <strsafe.h>



#if defined(OS_WIN)

#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable : 4996)
//#define _SECURE_SCL_DEPRECATE 0
//static constexpr int NO_WARN_MBCS_MFC_DEPRECATION
#pragma warning(push)
#pragma warning(disable : 4996)

// #include <Unknwn.h>

#include <Ws2tcpip.h>
#include <objidl.h>
#include <process.h>
#include <winsock2.h>
#include <winuser.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#include <gdiplus.h>
#include <minmax.h>

// Link to Mswsock.lib, Microsoft specific
#include <mswsock.h>
// Need to link with ws2_32.lib
// #pragma comment (lib, "Mswsock.lib")

#pragma comment(lib, "advapi32.lib") // provides security calls and functions for manipulating the Windows Registry
#pragma comment(lib, "comctl32.lib") //implements a wide variety of standard Windows controls
#pragma comment(lib, "comdlg32.lib") // Common Dialog Box Library
//#pragma comment(lib, "coredll.lib") // Windows CE
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "gdi32.lib") //perform primitive drawing functions for output to video displays and printers
#pragma comment(lib, "imm32.lib") // input method editor
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "kernel32.lib") //exposes to applications most of the Win32 Base APIs, such as memory management, input/output (I/O) operations, process and thread creation, and synchronization functions
#pragma comment(lib, "ksuser.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "netapi32.lib") // provides functions for querying and managing network interfaces.
#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "odbc32.lib")
#pragma comment(lib, "odbccp32.lib")
#pragma comment(lib, "ole32.lib") // provides the Component Object Model, as well as Object Linking and Embedding
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "shell32.lib")
//#pragma comment(lib, "shscrap.lib") // This functionality was removed from Windows Vista (and therefore later versions) to improve security. -Wikipedia
#pragma comment(lib, "user32.lib") // Windows USER component that creates and manipulates the standard elements of the Windows user interface, such as the desktop, windows, and menus
#pragma comment(lib, "userenv.lib")
#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "winmm.lib") // WinMM audio API
#pragma comment(lib, "winspool.lib")
#pragma comment(lib, "ws2_32.lib") // provides TCP/IP networking functions

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "shlwapi.lib")

#pragma comment(lib, "pcre.lib")


#include "shlwapi.h" // Deprecated API, https://msdn.microsoft.com/en-us/library/windows/desktop/bb759845(v=vs.85).aspx

//#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <Windowsx.h>
#include <tlhelp32.h>
#include <winnt.h> // LoadLibraryEx("library.dll", ...)

//#include <icu.h>

#include <conio.h>
#include <errno.h> /* for EINVAL */
#include <fcntl.h> /* for _O_TEXT and _O_BINARY */
#include <fcntl.h>
#include <share.h> /* for _SH_DENYNO */
#include <share.h>
#include <sys/locking.h>
#include <sys/stat.h> /* for _S_IWRITE */
#include <sys/stat.h>
#include <sys/types.h>

#include <direct.h>
#include <io.h>
#include <sys/stat.h>
//#include <sys/resource.h>
#include <psapi.h>
//#include <fileapi.h>
//#include <winioctl.h>
//#include "msvc-nothrow.h" // _get_osfhandle

//not #if defined(_WIN32) || defined(_WIN64) because we have strncasecmp in mingw
#define strncasecmp _strnicmp
#define strcasecmp _stricmp


#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif

// Operations for the `flock' call.
#define LOCK_SH 1 // Shared lock.
#define LOCK_EX 2 // Exclusive lock.
#define LOCK_UN 8 // Unlock.
// Can be OR'd in to one of the above.
#define LOCK_NB 4 // Don't block when locking.


#if defined(__MINGW32__)
#undef sprintf
#undef vsprintf
#undef strcpy
#endif

#endif

//#undef _MBCS

#undef snprintf

#undef _snprintf

//#define MAXBUFFERSIZE 100
//#define BIGBUFFERSIZE 5000

#if defined(OS_UNIX)
//typedef __int32 int;
//typedef unsigned int uint32_t;
//typedef __int64_t int64_t;
//typedef unsigned __int64_t uint64_t;
#endif







