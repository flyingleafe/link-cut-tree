#ifndef FLF_UTIL_H
#define FLF_UTIL_H

#include <experimental/optional>

template <typename V>
using maybe = std::experimental::optional<V>;

using nothing = std::experimental::bad_optional_access;

#endif
