#ifndef GEOM_SHAPE_HPP
#define GEOM_SHAPE_HPP

#include <vector>
#include <cstddef>

struct Point
{
  int x_, y_;
};

struct Polygon
{
  std::vector< Point > points_;
};

size_t getArea(const Polygon& polygon);

#endif
