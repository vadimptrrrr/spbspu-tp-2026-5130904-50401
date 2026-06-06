#ifndef GEOM_SHAPE_HPP
#define GEOM_SHAPE_HPP

#include <vector>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <limits>

namespace petrov
{
  namespace detail
  {
    struct Point
    {
      int x_, y_;
      bool operator>=(const Point& rhs) const;
      bool operator<=(const Point& rhs) const;
    };
    std::ostream& operator<<(std::ostream& os, const Point& point);
    std::istream& operator>>(std::istream& is, Point& point);
  }

  struct Polygon
  {
  std::vector< detail::Point > points_;
  };
  std::ostream& operator<<(std::ostream& os, const Polygon& polygon);
  std::istream& operator>>(std::istream& is, Polygon& polygon);
  void readPolygon(std::istream& is, std::vector< Polygon >& dst);
}
#endif
