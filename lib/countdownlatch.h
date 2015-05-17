#ifndef _LIB_COUNTDOWNLATCH_H_
#define _LIB_COUNTDOWNLATCH_H_

#include <boost/noncopyable.hpp>
#include "mutex.h"
#include "condition.h"

namespace zyflib
{
class CountDownLatch: boost::noncopyable
{
public:
    explicit CountDownLatch(int count); //倒数几次
    void wait();  //等待倒计时变为0
    void countDown();  //计数减一
    int getCount();
private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;

};//end of _LIB_COUNTDOWNLATCH_H_

}//end of zyflib


#endif //end of _LIB_COUNTDOWNLATCH_H_
