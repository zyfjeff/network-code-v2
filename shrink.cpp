/*
 =====================================================================================
        Filename:  shrink.cpp
     	Description:  
        Version:  1.0
        Created:  05/15/2015 05:26:15 AM
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> data(20);
    cout << data.size() << endl; //20
    cout << data.capacity() << endl; //20
    data.push_back(5);
    cout << data.capacity() << endl; //40
    data.shrink_to_fit();
//C++11支持的，缩小预留空间，让capacity==size
    cout << data.capacity() << endl; //21
}
