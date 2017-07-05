#ifndef DATASET_HPP_INCLUDED
#define DATASET_HPP_INCLUDED

#include <utility>
#include <vector>

namespace otherlib
{

class dataset
{
public:
  auto boundary() const -> std::vector<std::pair<int, int>>
  {
    return {{0, 0}, {2, 3}, {1, 2}, {1, 0}};
  }
};

} // namespace otherlib

#endif // DATASET_HPP_INCLUDED
