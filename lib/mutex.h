#ifndef _LIB_MUTEX_H_
#define _LIB_MUTEX_H_

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <boost/noncopyable.hpp>

pid_t gettid()
{
    return static_cast<pid_t>(::syscall(SYS_gettid));  
}

#define EXIT_ERROR(msg) do          \
{                                   \
    perror(msg);                    \
    exit(EXIT_FAILURE);             \
}while(0)


class MutexLock:public boost::noncopyable
{
public:
  explicit MutexLock()
            :holder_(0)
  {
      if (pthread_mutex_init(&mutex_,NULL) != 0)
            EXIT_ERROR("init mutex");       
  }
  ~MutexLock()
  {
      assert(holder_ == 0);
      if (pthread_mutex_destroy(&mutex_) != 0)
            EXIT_ERROR("destory mutex");
  }
  bool isLockedByThisThread()const
  {
        return holder_ == gettid();
  }

  void assertLocked()const
  {
        assert(isLockedByThisThread());
  }
    
  void lock()
  {
        if (pthread_mutex_lock(&mutex_) != 0)
            EXIT_ERROR("lock mutex");
        else
            assignHolder();
  }

  void unlock()
  {
        if (pthread_mutex_unlock(&mutex_) != 0)
            EXIT_ERROR("unlock mutex");
        else
            unassignHolder();
  }

  pthread_mutex_t* getPthreadMutex()
  {
        return &mutex_;    
  }
private:
  void unassignHolder()
  {
    holder_ = 0;
  }
  void assignHolder()
  {
    holder_ = gettid();    
  }
  friend class Condition;
  pthread_mutex_t mutex_; 
  pid_t holder_;
};


class MutexLockGuard:boost::noncopyable
{
public:
    explicit MutexLockGuard(MutexLock& mutex)
                            :mutex_(mutex)
    {
        mutex_.lock();    
    }
    ~MutexLockGuard()
    {
        mutex_.unlock();    
    }
private:
    MutexLock& mutex_;
};

#endif //end of _LIB_MUTEX_H_
