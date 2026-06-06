#include "commands.hpp"
#include <algorithm>
#include <numeric>


bool petrov::detail::hasEvenVert(const Polygon& p)
{
  return p.points_.size() % 2 == 0;
}

bool petrov::detail::hasOddVert(const Polygon& p)
{
  return !hasEvenVert(p);
}

double petrov::detail::getArea(const Polygon& p)
{
  if (p.points_.size() < 3)
  {
    return 0.0;
  }

  const auto& pts = p.points_;
  size_t n = pts.size();
  double sum = std::accumulate(p.points_.begin(), p.points_ .end(), 0.0,
    [&pts, n, i = size_t(0)](double acc, const detail::Point& curr) mutable {
      const auto& next = pts[(i + 1) % n];
      double cross_product = static_cast< double >(curr.x_) * next.y_ - static_cast< double >(next.x_) * curr.y_;
      return acc + cross_product;
    });

  return std::abs(sum) / 2.0;
}

double petrov::detail::sumArea(const std::vector< double >& dst)
{
  return std::accumulate(dst.begin(), dst.end(), 0.0);
}
