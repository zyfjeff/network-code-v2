/*
 =====================================================================================
        Filename:  gettimeofday.cpp
     	Description:  
        Version:  1.0
        Created:  05/19/2015 02:00:18 AM
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <iostream>
#include <string>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

using namespace std;

int main()
{
    struct timeval tm;
    if (gettimeofday(&tm,NULL) != 0) {
        perror("get time error\n");
        exit(-1);
    }
    int64_t sec = tm.tv_sec;
    int64_t usec = tm.tv_usec;
    cout << sec << endl;
    cout << usec << endl;
}
