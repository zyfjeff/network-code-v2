

#include <boost/operators.hpp>
#include <inttypes.h>
#include <boost/static_assert.hpp>
#include <string>
#include <stdio.h>
#include <sys/time.h>
#include "timestamp.h"

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
using namespace zyflib;
using namespace std;

Timestamp::Timestamp(int64_t microSeconds)
    :microSecondsSinceEpoch_(microSeconds)
{

}

string Timestamp::toString()const
{
    char buf[32] ={0};
    int64_t seconds = microSecondsSinceEpoch_/kMicroSecondsPerSecond;
    int64_t microSeconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
    snprintf(buf, sizeof(buf)-1, "%" PRId64 ".%06" PRId64 "", seconds, microSeconds);
    //snprintf(buf, sizeof(buf)-1, "%ld  %ld", seconds, microSeconds);
    return buf;
}

string Timestamp::toFormattedString(bool showMicroseconds)const
{
    char buf[32] = {0};
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_/kMicroSecondsPerSecond);
    struct tm tm_time;
    gmtime_r(&seconds,&tm_time);
    if (showMicroseconds) {
        int microSeconds = static_cast<int>(microSecondsSinceEpoch_%kMicroSecondsPerSecond);
        snprintf(buf,sizeof(buf),"%4d%02d%02d %02d:%02d:%02d.%06d",
                tm_time.tm_year + 1900,tm_time.tm_mon + 1,tm_time.tm_mday,
                tm_time.tm_hour,tm_time.tm_min,tm_time.tm_sec,
                microSeconds);
    } else {
        snprintf(buf,sizeof(buf),"%4d%02d%02d %02d:%02d:%02d",
                tm_time.tm_year + 1900,tm_time.tm_mon + 1,tm_time.tm_mday,
                tm_time.tm_hour,tm_time.tm_min,tm_time.tm_sec);
    }
    return buf;
}


Timestamp Timestamp::now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

Timestamp Timestamp::invalid()
{
    return Timestamp();   
}
