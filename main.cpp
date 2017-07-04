#include "square.hpp"
#include "dataset.hpp"

#include <vector>

int main() {
  const auto square = geometry::square{0.0, 0.0, 1.0};
  const auto dataset = otherlib::dataset{};

  auto polygons = std::vector<geometry::polygon<double>>();

  polygons.push_back(square);
  //polygons.push_back(dataset);
}
