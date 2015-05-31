/*
 =====================================================================================
        Filename:  algorithm_bound.cpp
     	Description:  lower_bound和upper_bound测试
        Version:  1.0
        Created:  05/31/2015 03:27:50 PM
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */
#include <algorithm>
#include <iostream>
#include <vector>


using namespace std;

template <typename T>
class PrintClass
{
    public:
            void operator()(T &data)
            {
                cout << data << endl;
            }
};
int main()
{
   vector<int>  data(10);
   for(int i = 0; i < 10; ++i)
   {
        data[i] = i;     
   }
//for_each遍历
   for_each(data.begin(),data.end(),PrintClass<int>());
   //返回第一个小于等于5的元素的迭代器
   vector<int>::iterator ret = lower_bound(data.begin(),data.end(),5);
   cout <<"lower_bound:" <<*ret << endl;
   //返回第一个大于5的元素的迭代器
   ret = upper_bound(data.begin(),data.end(),5);
   cout <<"upper_bound:" <<*ret << endl;
   pair<vector<int>::iterator,vector<int>::iterator> double_ret = equal_range(data.begin(),
                                                                            data.end(),4);

   cout <<"double_lower_bound:" <<*double_ret.first << endl;
   cout <<"double_upper_bound:" <<*double_ret.second << endl;
   cout << "-------------------------------" << endl;
   for_each(ret,data.end(),PrintClass<int>());
}
