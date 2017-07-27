#ifndef SQUARE_HPP_INCLUDED
#define SQUARE_HPP_INCLUDED

#include "polygon.hpp"

namespace geometry
{

class square : public convex_polygon<double>
{
public:
  square(double x, double y, double length) : x{x}, y{y}, length{length} {}

  auto vertices() const -> std::vector<coord<value_type>> override
  {
    return {{x, y}, {x + length, y}, {x + length, y + length}, {x, y + length}};
  }

private:
  double x, y;
  double length;
};

} // namespace geometry

#endif // SQUARE_HPP_INCLUDED
