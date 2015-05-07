/*
 =====================================================================================
        Filename:  algorithm_remove.cpp
     	Description:  STL 算法 remove的使用
        Version:  1.0
        Created:  05/08/2015 12:41:54 AM
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
        vector<int> data;
        for(int i = 0;i < 10;i++)
            data.push_back(i);


        //remove删除符合条件的值，并在尾后位置填充最后一个元素的值
        //返回指向最后一个元素的迭代器指针
        vector<int>::iterator tmp = std::remove(data.begin(),data.end(),9);
        cout << *tmp << endl;
        
        for(vector<int>::iterator it = data.begin(); it != data.end();++it)
            cout << *it << endl;
        cout << "-----------------------" << endl;
        
        //常常结合erase来删除元素
        data.erase(std::remove(data.begin(),data.end(),6),data.end());

        for(vector<int>::iterator it = data.begin(); it != data.end();++it)
            cout << *it << endl;
        
}
