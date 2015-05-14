/*
 =====================================================================================
        Filename:  thread_object.cpp
     	Description:  基于对象的编程风格
        Version:  1.0
        Created:  05/11/2015 11:15:31 PM
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <iostream>

using namespace std;


class Thread : boost::noncopyable
{
public:
    typedef boost::function<void()> ThreadFunc;
    explicit Thread(const ThreadFunc&);
    ~Thread();
    void Start();
    void Join();
    void SetAutoDelete(bool state);
private:
    static void* thread_routine(void *);
    pthread_t ThreadId_;
    bool autoDelete_;
    void Run();
    ThreadFunc func_;
};

Thread::Thread(const ThreadFunc& func):
        func_(func),
        autoDelete_(false),
        ThreadId_(0)
{
    cout << "thread construct" << endl;    
}

Thread::~Thread()
{
    cout << "thread destruct" << endl;    
}

void Thread::Start()
{
    pthread_create(&ThreadId_,NULL,thread_routine,this);    
}

void Thread::Join()
{
    pthread_join(ThreadId_,NULL);    
}

void* Thread::thread_routine(void *arg)
{
    Thread* thread = static_cast<Thread*>(arg);
    thread->Run();
    if (thread->autoDelete_)
        delete thread;
    return NULL;
}

void Thread::SetAutoDelete(bool state)
{
    autoDelete_ = state;    
}

void Thread::Run()
{
    func_();    
}

//线程要执行的函数，如果要传参数，可以通过bind对参数进行绑定即可。
void func()
{
    int count=10;
    while(count--)
    {
        cout << count << endl;
        sleep(1);
    }
}

int main()
{
    Thread *thread = new Thread(boost::bind(&func));
    thread->SetAutoDelete(true);
    thread->Start();
    thread->Join();
    return 0;
}
