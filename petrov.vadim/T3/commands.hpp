#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "geom_shape.hpp"

namespace petrov
{
  using pvec_t = std::vector< Polygon >;
  void area(std::istream& in, std::ostream& out, pvec_t& polygons);
  void max(std::istream& in, std::ostream& out, pvec_t& polygons);
  void min(std::istream& in, std::ostream& out, pvec_t& polygons);
  void count(std::istream& in, std::ostream& out, pvec_t& polygons);
  void intersection(std::istream& in, std::ostream& out, pvec_t& polygons);
  void same(std::istream& in, std::ostream& out, pvec_t& polygons);

  namespace detail
  {
    bool hasEvenVert(const Polygon& p);
    bool hasOddVert(const Polygon& p);
    size_t getArea(const Polygon& p);
    size_t sumArea(const std::vector< size_t >& dst);
  }
}

#endif
