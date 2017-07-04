#ifndef DATASET_HPP_INCLUDED
#define DATASET_HPP_INCLUDED

#include <vector>
#include <utility>

namespace otherlib
{
class dataset
{
public:
  auto boundary() const -> std::vector<std::pair<int, int>> { return {{0, 0}, {2, 3}, {1, 2}, {1, 0}}; }
};
}

#endif // DATASET_HPP_INCLUDED
