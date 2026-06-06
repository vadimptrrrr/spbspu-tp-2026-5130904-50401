#include "geom_shape.hpp"
#include "util.hpp"

std::ostream& petrov::detail::operator<<(std::ostream& os, const Point& point)
{
  std::ostream::sentry sentry(os);
  if (!sentry)
  {
    return os;
  }
  os << "(" << point.x_ << ";" << point.y_ << ")";
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

std::ostream& petrov::operator<<(std::ostream& os, const Polygon& polygon)
{
  std::ostream::sentry sentry(os);
  if (!sentry)
  {
    return os;
  }
  IOguard guard(os);

  size_t size = polygon.points_.size();
  os << size << ' ';
  using osit_t = std::ostream_iterator< detail::Point >;
  std::copy_n(polygon.points_.begin(), size - 1, osit_t{os, " "});
  os << polygon.points_[size-1];
  return os;
}

std::istream& petrov::operator>>(std::istream& is, Polygon& polygon)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }
  IOguard guard(is);

  size_t size = 0;
  is >> size;

  if (!is || size < 3)
  {
    is.setstate(std::ios_base::failbit);
    polygon.points_.clear();
    return is;
  }

  std::vector< detail::Point > tmp;
  tmp.reserve(size);
  using isit_t = std::istream_iterator< detail::Point >;
  std::copy_n(isit_t{is}, size, std::back_inserter(tmp));
  if (is && tmp.size() == size)
  {
    polygon.points_ = std::move(tmp);
  }
  else
  {
    is.setstate(std::ios_base::failbit);
    polygon.points_.clear();
  }

  return is;
}

void petrov::readPolygon(std::istream& is, std::vector< Polygon >& dst)
{
  Polygon p;
  is >> p;
  if (is.eof() && p.points_.empty())
  {
    return;
  }

  if (is.fail())
  {
    is.clear();
    is.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  else if (!p.points_.empty())
  {
    dst.push_back(p);
  }

  if (!is.eof())
  {
    readPolygon(is, dst);
  }
}

bool petrov::detail::Point::operator>(const petrov::detail::Point& rhs) const
{
  if (x_ != rhs.x_)
    {
      return x_ > rhs.x_;
    }
  return y_ > rhs.y_;
}
