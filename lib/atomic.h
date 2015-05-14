/*
 * =====================================================================================
 *
 *       Filename:  atomic.h
 *
 *    Description:  原子操作封装
 *
 *        Version:  1.0
 *        Created:  05/14/2015 02:24:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jeff (), zyfforlinux@163.com
 *   Organization:  Linux
 *
 * =====================================================================================
 */

#ifndef _LIB_ATMOTIC_H_
#define _LIB_ATMOTIC_H_
#include <stdint.h>
#include <boost/noncopyable.hpp>
namespace zyflib {

template<typename T>
class AtomicInteger : boost::noncopyable
{
  public:
    AtomicInteger():value(0)
    {   
    }
    //如果value值是0,则返回0,否则返回value本身
    T get()
    {
        return __sync_val_compare_and_swap(&value,0,0);    
    }
    //返回value的值，并让value+x
    T getAndAdd(T x)
    {
        return __sync_fetch_and_add(&value,x);    
    }
    //获取value+x的值
    T addAndGet(T x)
    {
        return getAndAdd(x) + x;
    }
    //递增
    T incrementAndGet()
    {
        return addAndGet(1);    
    }
    //递减
    T decrementAndGet()
    {
        return addAndGet(-1);    
    }
    //+x
    void add(T x)
    {
        getAndAdd(x);    
    }
    //递增
    void increment()
    {
        incrementAndGet();
    }
    //递减
    void decrement()
    {
        decrementAndGet();    
    }
    //赋值
    T getAndSet(T newValue)
    {
        return __sync_lock_test_and_set(&value,newValue);    
    }
 private:
    volatile T value;
}; //end of AtomicInteger
typedef zyflib::AtomicInteger<int32_t> AtomicInt32;
typedef zyflib::AtomicInteger<int64_t> AtomicInt64;
}//end of zyflib namespace 

#endif //end _LIB_ATMOTIC_H_
