#ifndef DATASET_HPP_INCLUDED
#define DATASET_HPP_INCLUDED

#include "polygon.hpp"

#include <vector>
#include <utility>

namespace otherlib
{

class dataset
{
  auto boundary() -> std::vector<std::pair<double, double>> { return {}; }
};

}

namespace geometry
{

template <> struct polygon_traits<otherlib::dataset> {
  using polygon_category_tag = simple_polygon_tag;
  using value_type = double;
};

}

#endif // DATASET_HPP_INCLUDED
