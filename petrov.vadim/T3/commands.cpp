#include "commands.hpp"
#include <algorithm>
#include <numeric>
#include <string>
#include <exception>
#include <iomanip>
#include <functional>
#include "util.hpp"


bool petrov::detail::hasEvenVert(const Polygon& p)
{
  return p.points_.size() % 2 == 0;
}

bool petrov::detail::hasOddVert(const Polygon& p)
{
  return !hasEvenVert(p);
}

bool petrov::detail::hasNVert(const Polygon& p, size_t count)
{
  return p.points_.size() == count;
}

double petrov::detail::getArea(const Polygon& p)
{
  if (p.points_.size() < 3)
  {
    return 0.0;
  }

  size_t n = p.points_.size();
  std::vector< size_t > idxs(n);
  std::iota(idxs.begin(), idxs.end(), 0);

  std::vector< double > products;
  products.reserve(n);
  std::transform(idxs.begin(), idxs.end(), std::back_inserter(products), [&p, n](size_t i)
  {
    const auto& curr = p.points_[i];
    const auto& next = p.points_[(i + 1) % n];
    return static_cast< double >(curr.x_) * next.y_ - static_cast< double >(next.x_) * curr.y_;
  });
  double sum = std::accumulate(products.begin(), products.end(), 0.0);

  return std::abs(sum) / 2.0;
}

double petrov::detail::sumArea(const std::vector< double >& dst)
{
  return std::accumulate(dst.begin(), dst.end(), 0.0);
}

void petrov::area(std::istream& in, std::ostream& out, pvec_t& polygons)
{
  std::string arg;
  in >> arg;
  if (!in)
  {
    throw std::runtime_error("Invalid read argument");
  }

  using namespace std::placeholders;
  IOguard guard(out);
  out << std::fixed << std::setprecision(1);
  if (arg == "EVEN")
  {
    pvec_t evenPol;
    std::vector< double > res;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(evenPol), detail::hasEvenVert);
    std::transform(evenPol.begin(), evenPol.end(), std::back_inserter(res), detail::getArea);
    out << detail::sumArea(res) << '\n';
  }
  else if (arg == "ODD")
  {
    pvec_t oddPol;
    std::vector< double > res;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(oddPol), detail::hasOddVert);
    std::transform(oddPol.begin(), oddPol.end(), std::back_inserter(res), detail::getArea);
    out << detail::sumArea(res) << '\n';
  }
  else if (arg == "MEAN")
  {
    if (polygons.size() < 1)
    {
      throw std::runtime_error("invalid polygon count");
    }

    std::vector< double > res;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(res), detail::getArea);
    out << detail::sumArea(res) / polygons.size() << '\n';
  }
  else
  {
    size_t n = 0;
    try
    {
      n = std::stoi(arg);
    }
    catch(const std::exception& e)
    {
      throw;
    }
    if (n < 3)
    {
      throw std::runtime_error("invalid arg");
    }
    pvec_t pol;
    std::vector< double > res;
    std::copy_if(polygons.begin(), polygons.end(), pol.begin(), std::bind(detail::hasNVert, _1, n));
    std::transform(pol.begin(), pol.end(), std::back_inserter(res), detail::getArea);
    out << detail::sumArea(res) << '\n';
  }
}

void petrov::max(std::istream& in, std::ostream& out, pvec_t& polygons)
{
  std::string arg;
  in >> arg;
  if (!in || polygons.empty())
  {
    throw std::runtime_error("Invalid read argument");
  }

  using namespace std::placeholders;
  IOguard guard(out);
  out << std::fixed << std::setprecision(1);

  if (arg == "AREA")
  {
    std::vector< double > areas;
    areas.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), detail::getArea);
    out << *std::max_element(areas.begin(), areas.end()) << '\n';
  }
  else if (arg == "VERTEXES")
  {
    std::vector< size_t > sizes;
    sizes.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(sizes),
      [](const Polygon& p) { return p.points_.size(); });
    out << *std::max_element(sizes.begin(), sizes.end()) << '\n';
  }
  else
  {
    throw std::runtime_error("invalid argument");
  }
}

void petrov::min(std::istream& in, std::ostream& out, pvec_t& polygons)
{
  std::string arg;
  in >> arg;
  if (!in || polygons.empty())
  {
    throw std::runtime_error("Invalid read argument");
  }

  using namespace std::placeholders;
  IOguard guard(out);
  out << std::fixed << std::setprecision(1);

  if (arg == "AREA")
  {
    std::vector< double > areas;
    areas.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), detail::getArea);
    out << *std::min_element(areas.begin(), areas.end()) << '\n';
  }
  else if (arg == "VERTEXES")
  {
    std::vector< size_t > sizes;
    sizes.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(sizes),
      [](const Polygon& p) { return p.points_.size(); });
    out << *std::min_element(sizes.begin(), sizes.end()) << '\n';
  }
  else
  {
    throw std::runtime_error("invalid argument");
  }
}

void petrov::count(std::istream& in, std::ostream& out, pvec_t& polygons)
{
  std::string arg;
  in >> arg;
  if (!in)
  {
    throw std::runtime_error("Invalid read argument");
  }

  if (arg == "EVEN")
  {
    out << std::count_if(polygons.begin(), polygons.end(), detail::hasEvenVert) << '\n';
  }
  else if (arg == "ODD")
  {
    out << std::count_if(polygons.begin(), polygons.end(), detail::hasOddVert) << '\n';
  }
  else
  {
    using namespace std::placeholders;
    size_t n = 0;
    try
    {
      n = std::stoi(arg);
    }
    catch(const std::exception& e)
    {
      throw;
    }
    if (n < 3)
    {
      throw std::runtime_error("invalid arg");
    }
    out << std::count_if(polygons.begin(), polygons.end(), std::bind(detail::hasNVert, _1, n)) << '\n';
  }
}

petrov::detail::Segment petrov::detail::makeSegment(const Point& p1, const Point& p2)
{
  return {p1, p2};
}

int petrov::detail::orient(const Point& a, const Point& b, const Point& c)
{
  return (b.x_ - a.x_) * (c.y_ - a.y_) - (b.y_ - a.y_) * (c.x_ - a.x_);
}

bool petrov::detail::onSegment(const Point& a, const Point& b, const Point& p)
{
  bool res = orient(a, b, p) == 0 && std::min(a.x_, b.x_) <= p.x_;
  res = res && p.x_ <= std::max(a.x_, b.x_) && std::min(a.y_, b.y_) <= p.y_ && p.y_ <= std::max(a.y_, b.y_);
  return res;
}

bool petrov::detail::segmentsIntersect(const Segment& s1, const Segment& s2)
{
  auto o1 = orient(s1.p1, s1.p2, s2.p1);
  auto o2 = orient(s1.p1, s1.p2, s2.p2);
  auto o3 = orient(s2.p1, s2.p2, s1.p1);
  auto o4 = orient(s2.p1, s2.p2, s1.p2);

  bool res = (o1 == 0 && onSegment(s1.p1, s1.p2, s2.p1)) || (o2 == 0 && onSegment(s1.p1, s1.p2, s2.p2));
  res = res || (o3 == 0 && onSegment(s2.p1, s2.p2, s1.p1)) || (o4 == 0 && onSegment(s2.p1, s2.p2, s1.p2));
  if (res)
  {
    return true;
  }

  return (o1 * o2 < 0) && (o3 * o4 < 0);
}

bool petrov::detail::intersectsWithAny(const Segment& seg, const std::vector< Segment >& segments)
{
  using namespace std::placeholders;
  return std::any_of(
    segments.begin(),
    segments.end(),
    std::bind(segmentsIntersect, seg, _1)
  );
}

std::vector< petrov::detail::Segment > petrov::detail::getSegments(const Polygon& pol)
{
  std::vector< Segment > res;
  res.reserve(pol.points_.size());
  
  std::transform(
    pol.points_.begin(),
    pol.points_.end() - 1,
    pol.points_.begin() + 1,
    std::back_inserter(res),
    makeSegment
  );
  
  res.push_back({pol.points_.back(), pol.points_.front()});
  return res;
}

bool petrov::detail::rayIntersectsSegment(const Point& p, const Segment& s)
{
  double py = static_cast< double >(p.y_);
  double y1 = static_cast< double >(s.p1.y_);
  double y2 = static_cast< double >(s.p2.y_);

  if (py < std::min(y1, y2) || py >= std::max(y1, y2))
  {
    return false;
  }
  if (y1 == y2)
  {
    return false;
  }

  double x1 = static_cast< double >(s.p1.x_);
  double x2 = static_cast< double >(s.p2.x_);

  double x_intersect = x1 + (py - y1) * (x2 - x1) / (y2 - y1);
  return x_intersect > p.x_;
}

bool petrov::detail::isPointInPolygon(const Point& p, const Polygon& pol)
{
  if (pol.points_.empty())
  {
    return false;
  }

  auto segs = getSegments(pol);
  using namespace std::placeholders;
  
  int intersections = std::count_if(
    segs.begin(),
    segs.end(),
    std::bind(rayIntersectsSegment, p, _1)
  );

  return (intersections % 2) != 0;
}

bool petrov::detail::polygonsIntersect(const Polygon& lhs, const Polygon& rhs)
{
  if (lhs.points_.empty() || rhs.points_.empty())
  {
    return false;
  }

  auto segs1 = getSegments(lhs);
  auto segs2 = getSegments(rhs);

  using namespace std::placeholders;
  if (std::any_of(segs1.begin(), segs1.end(), std::bind(intersectsWithAny, _1, std::cref(segs2))))
  {
    return true;
  }
  if (isPointInPolygon(lhs.points_.front(), rhs))
  {
    return true;
  }
  if (isPointInPolygon(rhs.points_.front(), lhs))
  {
    return true;
  }

  return false;
}

void petrov::intersection(std::istream& in, std::ostream& out, pvec_t& polygons)
{
  Polygon p;
  in >> p;
  if (!in || p.points_.empty())
  {
    throw std::runtime_error("invalid argument");
  }

  using namespace std::placeholders;
  
  out << std::count_if(polygons.begin(), polygons.end(), 
                      std::bind(detail::polygonsIntersect, std::ref(p), _1)) << '\n';
}

bool petrov::detail::checkMatch(const Polygon& lhs, const Polygon& rhs, size_t j, bool forward)
{
  size_t n = lhs.points_.size();
  std::vector< size_t > idxs(n);
  std::iota(idxs.begin(), idxs.end(), 0);

  return std::all_of(idxs.begin(), idxs.end(), [&](size_t i) {
    size_t rhs_idx = forward ? (j + i) % n : (j + n - i) % n;
    return (lhs.points_[i].x_ - lhs.points_[0].x_ == rhs.points_[rhs_idx].x_ - rhs.points_[j].x_) &&
           (lhs.points_[i].y_ - lhs.points_[0].y_ == rhs.points_[rhs_idx].y_ - rhs.points_[j].y_);
  });
}

bool petrov::detail::isSamePolygon(const Polygon& lhs, const Polygon& rhs)
{
  if (lhs.points_.size() != rhs.points_.size())
  {
    return false;
  }

  size_t n = lhs.points_.size();
  std::vector< size_t > idxs(n);
  std::iota(idxs.begin(), idxs.end(), 0);

  using namespace std::placeholders;
  
  bool fwd = std::any_of(idxs.begin(), idxs.end(), std::bind(checkMatch, std::cref(lhs), std::cref(rhs), _1, true));
  bool bwd = std::any_of(idxs.begin(), idxs.end(), std::bind(checkMatch, std::cref(lhs), std::cref(rhs), _1, false));

  return fwd || bwd;
}

void petrov::same(std::istream& in, std::ostream& out, pvec_t& polygons)
{
  Polygon p;
  in >> p;
  if (!in || p.points_.empty())
  {
    throw std::runtime_error("invalid argument");
  }

  using namespace std::placeholders;
  
  out << std::count_if(polygons.begin(), polygons.end(),
                       std::bind(detail::isSamePolygon, std::ref(p), _1)) << '\n';
}
