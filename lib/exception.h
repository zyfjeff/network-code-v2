#ifndef _LIB_EXCEPTION_H_
#define _LIB_EXCEPTION_H_

#include <boost/noncopyable.hpp>
#include <exception>
#include <string>
using namespace std;

namespace zyflib
{
class Exception : public std::exception
{
public:
    explicit Exception(const char *what);
    explicit Exception(const string &what);
    virtual ~Exception()throw();
    virtual const char *what()const throw();
    const char* stackTrace() const throw();
  private:
    void fillStackTrace();
    string message_;
    string stack_;
};
} //end of namespace zyflib

#endif  //end of _LIB_EXCEPTION_H_
