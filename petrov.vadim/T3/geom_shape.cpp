#include "geom_shape.hpp"
#include "util.hpp"

std::ostream& petrov::detail::operator<<(std::ostream& os, const Point& point)
{
  std::ostream::sentry sentry(os);
  if (!sentry)
  {
    return os;
  }
  os << "(" << point.x_ << ";" << point.y_ << "(";
  return os;
}

std::istream& petrov::detail::operator>>(std::istream& is, Point& point)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  IOguard guard(is);
  detail::Point tmp{0, 0};
  using dt_t = DelimeterIO;
  is >> dt_t{'('} >> tmp.x_ >> dt_t{';'} >> tmp.y_ >> dt_t{')'};
  if (is)
  {
    point = tmp;
  }
  return is;
}
