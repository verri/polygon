#ifndef META_HPP_INCLUDED
#define META_HPP_INCLUDED

#include <type_traits>


namespace meta
{

template <typename T, template <typename> typename Expression, typename = std::void_t<>>
struct compiles : std::false_type {};

template <typename T, template <typename> typename Expression>
struct compiles<T, Expression, std::void_t<Expression<T>>> : std::true_type {};

template <typename...> struct always_true : std::true_type {};

template <typename...> struct always_false : std::false_type {};

}

#endif // META_HPP_INCLUDED
