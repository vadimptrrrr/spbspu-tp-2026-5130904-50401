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
      std::cerr << e.what() << '\n';
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
