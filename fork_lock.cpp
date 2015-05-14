/*
 =====================================================================================
        Filename:  fork_lock.cpp
     	Description:  fork导致的死锁，并通过pthread_atfork来解决
        Version:  1.0
        Created:  05/15/2015 06:33:16 AM
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

//因为fork复制了锁的状态，导致子进程死锁了
#include <iostream>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* doit(void *data)
{
    printf("pid = %d begin doit ...\n",static_cast<int>(getpid()));
    pthread_mutex_lock(&mutex);
    struct timespec sp = {2,0};
    nanosleep(&sp,NULL);
    pthread_mutex_unlock(&mutex);
    printf("pid = %d end doit ...\n",static_cast<int>(getpid()));
}

int main()
{
    printf("pid = %d Entering main ...\n", static_cast<int>(getpid()));
    pthread_t p;
    pthread_create(&p,NULL,doit,NULL);
    struct timespec ts = {1,0};
    nanosleep(&ts,NULL);
    if (fork() == 0) {
        doit(NULL);//线程中已经加锁了，导致子进程执行的时候锁的状态是锁定的，
                   //所以导致子进程再次加锁出现死锁。
    }
    pthread_join(p,NULL);
    printf("pid = %d Exiting main ...\n",static_cast<int>(getpid()));
}
