#include "dataset.hpp"
#include "square.hpp"

#include <algorithm>
#include <memory>
#include <vector>
#include <iostream>

namespace geometry
{

class dataset_wrapper : public simple_polygon<double>, public otherlib::dataset
{
public:
  dataset_wrapper(otherlib::dataset dataset) : otherlib::dataset(std::move(dataset)) {}

  auto vertices() const -> std::vector<coord<value_type>> override {
    auto values = boundary();
    auto result = std::vector<coord<value_type>>();

    result.reserve(values.size());
    std::transform(values.begin(), values.end(), std::back_inserter(result),
                   [](const auto& p) -> coord<value_type> {
                     return {static_cast<double>(p.first), static_cast<double>(p.second)};
                   });

    return result;
  }
};

template <typename T>
auto is_inside(coord<T>, const simple_polygon<T>&)
{
   std::cout << "calling simple overload\n";
}

template <typename T>
auto is_inside(coord<T>, const convex_polygon<T>&)
{
  std::cout << "calling convex overload\n";
}

template <typename T>
auto is_inside(coord<T>, const concave_polygon<T>&)
{
  std::cout << "calling concave overload\n";
}

template <typename T> auto is_inside(coord<T>, const general_polygon<T>& pol)
{
  std::cout << "virtual dispatching...\n";

  if (auto p = dynamic_cast<const convex_polygon<T>*>(&pol))
    std::cout << "calling convex overload\n";
  else if (auto p = dynamic_cast<const concave_polygon<T>*>(&pol))
    std::cout << "calling concave overload\n";
  else if (auto p = dynamic_cast<const simple_polygon<T>*>(&pol))
    std::cout << "calling simple overload\n";
  else
    throw std::runtime_error{"wrong input"};
}

} // namespace geometry

int main()
{
  const auto square = geometry::square{0.0, 0.0, 2.0};
  const auto dataset = otherlib::dataset{};

  auto polygons = std::vector<std::unique_ptr<geometry::general_polygon<double>>>();

  polygons.push_back(std::make_unique<geometry::square>(square));
  polygons.push_back(std::make_unique<geometry::dataset_wrapper>(dataset));

  using geometry::is_inside;

  is_inside({1.0, 1.0}, square);
  is_inside({1, 1}, geometry::dataset_wrapper{dataset});

  for (auto&& pol : polygons)
    is_inside({1.0, 1.0}, *pol);

  for (auto&& pol : polygons)
  {
    for (const auto& point : pol->vertices())
      std::cout << point.x << ' ' << point.y << ", ";
    std::cout << '\n';
  }
}
