#include "util.hpp"

petrov::IOguard::IOguard(std::basic_ios< char >& s):
  s_(s),
  width_(s.width()),
  precision_(s.precision()),
  flags_(s.flags()),
  fill_(s.fill())
{}

petrov::IOguard::~IOguard()
{
  s_.precision(precision_);
  s_.width(width_);
  s_.flags(flags_);
  s_.fill(fill_);
}

std::istream& petrov::operator>>(std::istream& in, DelimeterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c;
  in >> c;
  if (c != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}
