#ifndef SQUARE_HPP_INCLUDED
#define SQUARE_HPP_INCLUDED

#include "polygon.hpp"

namespace geometry
{

struct square
{
  using polygon_category_tag = convex_polygon_tag;
  using value_type = double;

  double x, y;
  double length;

  auto vertices() const -> std::vector<coord<value_type>>
  {
    return {{x, y}, {x + length, y}, {x + length, y + length}, {x, y + length}};
  }
};

} // namespace geometry

#endif // SQUARE_HPP_INCLUDED
