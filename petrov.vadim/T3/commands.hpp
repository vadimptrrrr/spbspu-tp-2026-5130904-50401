#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include "geom_shape.hpp"

namespace petrov
{
  using pvec_t = std::vector< Polygon >;
  void area(std::istream& in, std::ostream& out, const pvec_t& polygons);
  void max(std::istream& in, std::ostream& out, const pvec_t& polygons);
  void min(std::istream& in, std::ostream& out, const pvec_t& polygons);
  void count(std::istream& in, std::ostream& out, const pvec_t& polygons);
  void intersection(std::istream& in, std::ostream& out, const pvec_t& polygons);
  void same(std::istream& in, std::ostream& out, const pvec_t& polygons);

  using cmd_t = std::function< void(std::istream&, std::ostream&, const pvec_t&) >;
  struct CommandExecuter {
    std::unordered_map<std::string, cmd_t > cmds;
    std::ostream& out;
    const pvec_t& ps;
  };
  std::istream& operator>>(std::istream&, CommandExecuter& cmd);

  namespace detail
  {
    struct Segment
    {
      Point p1;
      Point p2;
    };

    bool hasEvenVert(const Polygon& p);
    bool hasOddVert(const Polygon& p);
    bool hasNVert(const Polygon& p, size_t count);
    double getArea(const Polygon& p);
    double sumArea(const std::vector< double >& dst);

    Segment makeSegment(const Point& p1, const Point& p2);
    int orient(const Point& a, const Point& b, const Point& c);
    bool onSegment(const Point& a, const Point& b, const Point& p);
    bool segmentsIntersect(const Segment& s1, const Segment& s2);
    bool intersectsWithAny(const Segment& seg, const std::vector< Segment >& segments);
    std::vector< Segment > getSegments(const Polygon& pol);
    bool rayIntersectsSegment(const Point& p, const Segment& s);
    bool isPointInPolygon(const Point& p, const Polygon& pol);
    bool polygonsIntersect(const Polygon& lhs, const Polygon& rhs);

    bool checkMatch(const Polygon& lhs, const Polygon& rhs, size_t j, bool forward);
    bool isSamePolygon(const Polygon& lhs, const Polygon& rhs);
    bool isGarbage(std::istream& in);
  }
}

#endif
