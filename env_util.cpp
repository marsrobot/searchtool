#if defined _WIN32 || defined _WIN32_
#include <windows.h>
#endif

#include "macro.h"
#include "env_util.h"

int setenv_u(const char* key, const char* val, int overwrite)
{
#if defined(OS_UNIX)
	return setenv(key, val, (int)overwrite);
#else
	return _putenv_s(key, val);
#endif
}
