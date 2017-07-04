#include "square.hpp"
#include "dataset.hpp"

#include <algorithm>
#include <cassert>
#include <vector>
#include <iostream>

namespace geometry
{
template <> struct polygon_traits<otherlib::dataset> {
  using polygon_category_tag = simple_polygon_tag;
  using value_type = int;
};
}

namespace otherlib
{
auto vertices(const dataset& dataset) {
  const auto boundary = dataset.boundary();

  auto vs = std::vector<geometry::coord<int>>();
  vs.reserve(boundary.size());

  std::transform(boundary.begin(), boundary.end(), std::back_inserter(vs),
    [](const auto& pair) -> geometry::coord<int> {
      return {pair.first, pair.second};
    });

  return vs;
}
}

namespace geometry
{
template <typename Polygon>
auto is_inside(coord<typename polygon_traits<Polygon>::value_type>, const Polygon&, simple_polygon_tag)
{
  std::cout << "calling simple overload\n";
}

template <typename Polygon>
auto is_inside(coord<typename polygon_traits<Polygon>::value_type>, const Polygon&, convex_polygon_tag)
{
  std::cout << "calling convex overload\n";
}

template <typename Polygon>
auto is_inside(coord<typename polygon_traits<Polygon>::value_type>, const Polygon&, concave_polygon_tag)
{
  std::cout << "calling concave overload\n";
}

template <typename Polygon, typename Tag = typename polygon_traits<Polygon>::polygon_category_tag>
auto is_inside(coord<typename polygon_traits<Polygon>::value_type> point, const Polygon& pol)
{
  std::cout << "dispatching...\n";
  return is_inside(point, pol, Tag{});
}

template <typename T> auto is_inside(coord<T>, const polygon<T>& pol)
{
  std::cout << "virtual dispatching...\n";

  if (pol.is_convex())
    std::cout << "calling convex overload\n";
  else if (pol.is_concave())
    std::cout << "calling concave overload\n";
  else if (pol.is_simple())
    std::cout << "calling simple overload\n";
  else
    throw std::runtime_error{"wrong input"};
}

}

int main() {
  const auto square = geometry::square{0.0, 0.0, 2.0};
  const auto dataset = otherlib::dataset{};

  auto polygons = std::vector<geometry::polygon<double>>();

  polygons.push_back(square);
  polygons.push_back(dataset);

  using geometry::is_inside;

  is_inside({1.0, 1.0}, square);
  is_inside({1, 1}, dataset);

  for (const auto& pol : polygons)
    is_inside({1.0, 1.0}, pol);

  assert(polygons[0].category() == typeid(geometry::convex_polygon_tag));
  assert(polygons[1].category() == typeid(geometry::simple_polygon_tag));

  for (const auto& pol : polygons) {
    for (const auto& point : pol.vertices())
      std::cout << point.x << ' ' << point.y << ", ";
    std::cout << '\n';
  }
}
