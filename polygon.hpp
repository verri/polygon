#ifndef POLYGON_HPP_INCLUDED
#define POLYGON_HPP_INCLUDED

#include "coord.hpp"

#include <algorithm>
#include <vector>
#include <memory>
#include <typeindex>

namespace geometry
{

// Polygon traits
struct general_polygon_tag {};
struct non_convex_polygon_tag : general_polygon_tag {};
struct simple_polygon_tag : general_polygon_tag {};
struct convex_polygon_tag : simple_polygon_tag {};
struct concave_polygon_tag : non_convex_polygon_tag, simple_polygon_tag {};
struct self_intersecting_polygon_tag : general_polygon_tag {};

template <typename Polygon, typename = void> struct polygon_traits {};

template <typename Polygon>
struct polygon_traits<Polygon, std::enable_if_t<std::conjunction_v<
    std::is_base_of<general_polygon_tag, typename Polygon::polygon_category_tag>,
    std::is_arithmetic<typename Polygon::value_type>
  >>>
{
  using polygon_category_tag = typename Polygon::polygon_category_tag;
  using value_type = typename Polygon::value_type;
};

// Utilities
namespace detail
{

template <typename Polygon, typename = void>
struct has_free_vertices : std::false_type {};

template <typename Polygon>
struct has_free_vertices<Polygon, std::void_t<decltype(vertices(std::declval<const Polygon&>()))>> :
  std::true_type {};

template <typename Polygon>
static constexpr auto has_free_vertices_v = has_free_vertices<Polygon>::value;

template <typename Polygon, typename Point = coord<typename polygon_traits<Polygon>::value_type>>
auto invoke_vertices(const Polygon& polygon) -> std::vector<Point>
{
  if constexpr (has_free_vertices_v<Polygon>)
    return vertices(polygon);
  else
    return polygon.vertices();
}

} // namespace detail

// Type-erased polygon
template <typename T>
class polygon
{
public:
  using value_type = T;

private:
  class interface
  {
  public:
    virtual ~interface() = default;
    virtual auto clone() const -> std::unique_ptr<interface> = 0;

    virtual auto is_convex() const -> bool = 0;
    virtual auto is_concave() const -> bool = 0;
    virtual auto is_simple() const -> bool = 0;

    virtual auto vertices() const -> std::vector<coord<value_type>> = 0;

    virtual auto underlying_type() const -> std::type_index = 0;

    virtual auto category() const -> std::type_index = 0;

    template <typename Polygon> auto downcast() -> Polygon& { return dynamic_cast<Polygon&>(*this); }
    template <typename Polygon> auto downcast() const -> const Polygon& { return dynamic_cast<const Polygon&>(*this); }
  };

  template <typename Polygon> class erased : public interface
  {
  public:
    erased(Polygon polygon) : polygon_{std::move(polygon)} {}

    auto clone() const -> std::unique_ptr<interface> override {
      return std::make_unique<erased<Polygon>>(polygon_);
    }

    auto is_convex() const -> bool override {
      return std::is_base_of_v<convex_polygon_tag, typename polygon_traits<Polygon>::polygon_category_tag>;
    }

    auto is_concave() const -> bool override {
      return std::is_base_of_v<concave_polygon_tag, typename polygon_traits<Polygon>::polygon_category_tag>;
    }

    auto is_simple() const -> bool override {
      return std::is_base_of_v<simple_polygon_tag, typename polygon_traits<Polygon>::polygon_category_tag>;
    }

    auto vertices() const -> std::vector<coord<value_type>> override {
      if constexpr (std::is_same_v<value_type, typename polygon_traits<Polygon>::value_type>)
      {
        return detail::invoke_vertices(polygon_);
      }
      else
      {
        auto vs = detail::invoke_vertices(polygon_);

        auto result = std::vector<coord<value_type>>();
        result.reserve(vs.size());

        std::transform(vs.begin(), vs.end(), std::back_inserter(result),
          [](const auto& c) -> coord<value_type> {
            return {static_cast<value_type>(c.x), static_cast<value_type>(c.y)};
          });

        return result;
      }
    }

    auto underlying_type() const -> std::type_index override {
      return typeid(Polygon);
    }

    auto category() const -> std::type_index override {
      return typeid(typename polygon_traits<Polygon>::polygon_category_tag);
    }

  private:
    Polygon polygon_;
  };

public:
  polygon() = delete;

  template <typename Polygon> polygon(Polygon data) : data_{std::make_unique<erased<Polygon>>(std::move(data))} {}

  polygon(const polygon& source) : data_{source.data_->clone()} {}

  polygon(polygon&& source) = default;

  auto operator=(const polygon& source) -> polygon&
  {
    data_ = source.data_->clone();
    return *this;
  }

  auto operator=(polygon&& source) -> polygon& = default;

  decltype(auto) is_convex() const { return data_->is_convex(); }

  decltype(auto) is_concave() const { return data_->is_concave(); }

  decltype(auto) is_simple() const { return data_->is_simple(); }

  decltype(auto) vertices() const { return data_->vertices(); };

  decltype(auto) underlying_type() const { return data_->underlying_type(); }

  template <typename Polygon> decltype(auto) downcast() { return data_->template downcast<Polygon>(); }

  template <typename Polygon> decltype(auto) downcast() const { return data_->template downcast<Polygon>(); }

  decltype(auto) category() const { return data_->category(); }

private:
  std::unique_ptr<interface> data_;
};

}


#endif // POLYGON_HPP_INCLUDED
