/*
 =====================================================================================
        Filename:  gcc_thread.cpp
     	Description:  gcc内置的线程本地存储
        Version:  1.0
        Created:  05/15/2015 12:22:46 PM
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <iostream>
#include <stdio.h>
using namespace std;
__thread int zhang = 6;

void* handle(void *data)
{
    cout <<"pthread:"<<*(int*)data << endl;
    cout << ++zhang << endl;    
    cout << zhang << endl;    
}

int main()
{
    pthread_t tid1,tid2;
    int data1 = 1;
    int data2 = 2;
    pthread_create(&tid1,NULL,handle,&data1);
    pthread_create(&tid2,NULL,handle,&data2);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
}
