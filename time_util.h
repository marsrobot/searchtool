#pragma once

#include <cassert>
#include <chrono>

#include "macro.h"
#include "env_util.h"

static const int64_t MICRO_SECOND_RATIO = 1000000;
typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> TimePoint;
typedef int64_t microsecond_t;

int time_t_to_microsecond(time_t* ut, microsecond_t* usecs);
int unix_datetime_from_time_t(UnixDateTime* self, time_t* t);
int unix_datetime_get_data(const UnixDateTime* self, time_t* res);
time_t unix_datetime_get_data(const UnixDateTime* self);

int gettimeofday_u(struct timeval * tp, struct timezone * tzp);

int datetime_now_in_microsecond(microsecond_t* res);
int microsecond_to_timeval(int64_t _usec, timeval* res);

int timeval_to_timepoint(struct timeval* tv, TimePoint* tp);

int microsecond_to_timepoint(int64_t _usec, TimePoint* res);

int microsecond_to_timestamp(int64_t _usec, std::string* res);

int datetime_now_as_timestamp(std::string* res);
