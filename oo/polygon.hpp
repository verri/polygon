#ifndef POLYGON_HPP_INCLUDED
#define POLYGON_HPP_INCLUDED

#include "coord.hpp"

namespace geometry
{

template <typename T> class general_polygon {
public:
  using value_type = T;
  virtual auto vertices() const -> std::vector<coord<value_type>> = 0;
};

template <typename T> class non_convex_polygon : public virtual general_polygon<T> {};
template <typename T> class simple_polygon : public virtual general_polygon<T> {};
template <typename T> class convex_polygon : public simple_polygon<T> {};
template <typename T> class concave_polygon : public non_convex_polygon<T>, simple_polygon<T> {};
template <typename T> class self_intersecting_polygon : public general_polygon<T> {};

} // namespace geometry

#endif // POLYGON_HPP_INCLUDED
