/*
 =====================================================================================
        Filename:  pthread_local.cpp
     	Description:  线程特定数据的使用
        Version:  1.0
        Created:  05/15/2015 06:01:29 AM
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

//利用线程特定数据，让每个线程访问和操作自己的数据副本。
#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
static pthread_key_t key;
pthread_mutex_t env_mutex = PTHREAD_MUTEX_INITIALIZER;
int data = 10; //全局数据,让两个线程都绑定上

void* handle(void *data)
{
    int *key_data;
    pthread_setspecific(key,&data); //需要自己绑定数据
    key_data =(int*)pthread_getspecific(key); //需要使用这个来获取绑定的数据
    *key_data = *(int*)data;
    cout <<"thread:" <<*key_data << endl;
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid1,tid2;
    int data1 = 2;
    int data2 = 3;
    pthread_key_create(&key,NULL); //使用pthread_once来确保只调用一次   
    pthread_create(&tid1,NULL,handle,(void*)&data1);
    pthread_create(&tid2,NULL,handle,(void*)&data2);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
}
