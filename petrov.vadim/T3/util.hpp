#ifndef UTIL_HPP
#define UTIL_HPP

#include <iomanip>
namespace petrov
{
  class IOguard
  {
  public:
    explicit IOguard(std::basic_ios< char >& s);
    ~IOguard();

  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags flags_;
    char fill_;
  };

  struct DelimeterIO
  {
    char exp;
  };
  std::istream& operator>>(std::istream& in, DelimeterIO&& dest);
}

#endif
