#ifndef _LIB_TIMESTAMP_H_
#define _LIB_TIMESTAMP_H_

#include <boost/operators.hpp>
#include <inttypes.h>
#include <boost/static_assert.hpp>
#include <string>

namespace zyflib
{
using namespace std;
class Timestamp : public boost::less_than_comparable<Timestamp>
{
public:
    Timestamp()
        :microSecondsSinceEpoch_(0)
    {
    }
    explicit Timestamp(int64_t microSecondsSinceEpoch);
    void swap(Timestamp& that)
    {
        std::swap(microSecondsSinceEpoch_,that.microSecondsSinceEpoch_);    
    }
    string toString()const;
    string toFormattedString(bool showMicroseconds = true)const;
    bool valid()const {return microSecondsSinceEpoch_ > 0;}
    int64_t microSecondsSinceEpoch()const {return microSecondsSinceEpoch_;}
    time_t secondsSinceEpoch()const
    {
        return static_cast<time_t>(microSecondsSinceEpoch_/kMicroSecondsPerSecond);
    }
    static Timestamp now();
    static Timestamp invalid();
    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    int64_t microSecondsSinceEpoch_;
};
} //end of zyflib namespace
#endif //end of timestamp.h
