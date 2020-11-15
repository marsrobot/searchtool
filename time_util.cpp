
#if defined(_WIN32) || defined(_WIN32_)
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <string>
#include <time.h>
#include "time_util.h"
#include "macro.h"

int time_t_to_microsecond(time_t* ut, microsecond_t* usecs)
{
    *usecs = (*ut) * MICRO_SECOND_RATIO;
    return 0;
}

int unix_datetime_from_time_t(UnixDateTime* self, time_t* t)
{
    self->_time = *t;
    return 0;
}

int unix_datetime_get_data(const UnixDateTime* self, time_t* res)
{
    *res = self->_time;
    return 0;
}

time_t unix_datetime_get_data(const UnixDateTime* self)
{
    time_t res;
    res = self->_time;
    return res;
}

#if defined(OS_UNIX)
int gettimeofday_u(struct timeval* tp, struct timezone* tzp)
{
    return gettimeofday(tp, tzp);
}
#else
int gettimeofday_u(struct timeval* tp, struct timezone* tzp)
{
    static const unsigned __int64 epoch = uint64_t(116444736000000000);
    FILETIME    file_time;
    SYSTEMTIME  system_time;
    ULARGE_INTEGER ularge;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    ularge.LowPart = file_time.dwLowDateTime;
    ularge.HighPart = file_time.dwHighDateTime;

    tp->tv_sec = (long)((ularge.QuadPart - epoch) / 10000000L);
    tp->tv_usec = (long)(system_time.wMilliseconds * 1000);

    return 0;
}
#endif

int datetime_now_in_microsecond(microsecond_t* res)
{
    struct timeval tv;
    gettimeofday_u(&tv, nullptr);
    *res = tv.tv_sec * MICRO_SECOND_RATIO + tv.tv_usec;
    return 0;
}

int microsecond_to_timeval(int64_t _usec, timeval* res)
{
    timeval tv;
    tv.tv_sec = _usec / MICRO_SECOND_RATIO;
    tv.tv_usec = _usec % MICRO_SECOND_RATIO;
    *res = tv;
    return 0;
}

int timeval_to_timepoint(struct timeval* tv, TimePoint* tp)
{
    *tp = TimePoint(std::chrono::microseconds{ tv->tv_sec * MICRO_SECOND_RATIO + tv->tv_usec });
    return 0;
}

int microsecond_to_timepoint(int64_t _usec, TimePoint* res)
{
    timeval tv;
    microsecond_to_timeval(_usec, &tv);
    timeval_to_timepoint(&tv, res);
    return 0;
}

int microsecond_to_timestamp(int64_t _usec, std::string* res)
{
    assert(_usec > 0);
    TimePoint tp;
    microsecond_to_timepoint(_usec, &tp);
    auto micros = (tp.time_since_epoch() / std::chrono::microseconds(1)) % MICRO_SECOND_RATIO;
    time_t tt = std::chrono::system_clock::to_time_t(tp);

    char buf[80];
    // setenv_u("TZ", "PST8PDT", 1);
    setenv_u("TZ", "America/New_York", 1);
    struct tm* timeinfo = localtime(&tt);
    // strftime(buffer, 80, "%F %H:%M:%S", timeinfo); // %F is from C99 and not
    // supported in Visual C++.
    strftime(buf, 80, "%Y-%m-%dT%H:%M:%S", timeinfo);

    char buf_ts[80];
    sprintf(buf_ts, "%s.%06lld", buf, (long long)micros);
    *res = std::string(buf_ts);
    return 0;
}

int datetime_now_as_timestamp(std::string* res)
{
    microsecond_t usec;
    datetime_now_in_microsecond(&usec);
    microsecond_to_timestamp(usec, res);
    return 0;
}

