/*
 =====================================================================================
        Filename:  exception.cpp
     	Description:  
        Version:  1.0
        Created:  05/14/2015 09:10:34 PM
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */
#include "exception.h"
#include <execinfo.h>
#include <stdlib.h>

using namespace zyflib;

Exception::Exception(const char *msg)
                    :message_(msg)
{
    fillStackTrace();    
}

Exception::Exception(const string &msg)
                    :message_(msg)
{
    fillStackTrace();    
}

Exception::~Exception()throw()
{
        
}

const char* Exception::what()const throw()
{
    return message_.c_str();    
}

const char* Exception::stackTrace()const throw()
{
    return stack_.c_str();    
}

void Exception::fillStackTrace()
{
    const int len = 200;
    void *buffer[len];
    int nptrs = ::backtrace(buffer,len);
    char** strings = ::backtrace_symbols(buffer,nptrs);
    if (strings)
    {
        for(int i = 0;i < nptrs;++i)
        {
            stack_.append(strings[i]);
            stack_.push_back('\n');
        }
        free(strings);
    }
}

string Exception::demangle(const char* symbol)
{
  size_t size;
  int status;
  char temp[128];
  char* demangled;
  //first, try to demangle a c++ name
  if (1 == sscanf(symbol, "%*[^(]%*[^_]%127[^)+]", temp)) {
    if (NULL != (demangled = abi::__cxa_demangle(temp, NULL, &size, &status))) {
      string result(demangled);
      free(demangled);
      return result;
    }
  }
  //if that didn't work, try to get a regular c symbol
  if (1 == sscanf(symbol, "%127s", temp)) {
    return temp;
  }
 
  //if all else fails, just return the symbol
  return symbol;
}
