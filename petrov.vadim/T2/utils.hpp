#include <string>
#include <iostream>
#include <iomanip>

namespace petrov
{
  struct DataStruct
  {
    double key1;
    char key2;
    std::string key3;
  };

  struct DelimeterIO
  {
    char exp;
  };

  struct CharIO
  {
    char& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct DobleLitIO
  {
    double& ref;
  };

  struct LabelIO
  {
    std::string& ref;
  };

  class IOguard
  {
  public:
    explicit IOguard(std::basic_ios< char > & s);
    ~IOguard();

  private:
    std::basic_ios< char > & s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags flags_;
    char fill_;
  };

  std::istream& operator>>(std::istream& in, DelimeterIO&& dest);
  std::istream& operator>>(std::istream& in, CharIO&& dest);
  std::istream& operator>>(std::istream& in, LabelIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, DobleLitIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);

  std::ostream& operator<<(std::ostream& out, const DataStruct& data);

  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}
