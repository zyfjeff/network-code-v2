/*
 =====================================================================================
        Filename:  thread_oo.cpp
     	Description:  面向对象的线程封装
        Version:  1.0
        Created:  05/11/2015 10:45:12 PM
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <iostream>
#include <pthread.h>

using namespace std;

class Thread
{
public:
    Thread();
    virtual ~Thread(); //虚析构,目的是在子类析构的时候，可以调用父类的析构，避免内存泄露
    void Start(); //启动线程
    void Join();  //等待线程结束
    void SetAutoDelete(bool state); //设置autoDelete状态
private:
    static void* thread_routine(void *arg); //pthread_create需要设置的函数指针，
    virtual void Run() = 0;  //接口，实现多态。
    pthread_t threadId_; //保存线程ID
    bool autoDelete;  //标志是否在线程执行完毕后自动释放线程对象
};

void Thread::Join()
{
    pthread_join(threadId_,NULL);
}

void Thread::SetAutoDelete(bool state)
{
    autoDelete = state;    
}

void * Thread::thread_routine(void *arg)
{
    Thread *thread = static_cast<Thread*>(arg);
    thread->Run();
    if(thread->autoDelete)
        delete thread;
    return NULL;
}

Thread::Thread():autoDelete(false)
{
    cout << "Thread..." << endl;
}

void Thread::Start()
{
    pthread_create(&threadId_,NULL,thread_routine,this);
}

Thread::~Thread()
{
  cout << "~Thread..." << endl;      
}

class TestThread:public Thread
{
  public:
    TestThread(int count):count_(count)
    {
        cout << "testthread...." << endl; 
    }
    ~TestThread()
    {
        cout << "~TestThread..." << endl;    
    }
  private:
    void Run()
    {
        while(count_--)
        {
            cout << "this is a test" << endl;
            sleep(1);
        }
    }
    int count_;
};

int main()
{
    TestThread *t2 = new TestThread(10);
    t2->SetAutoDelete(true);
    t2->Start();
    t2->Join();
    return 0;
}
