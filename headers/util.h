#ifndef FLF_UTIL_H
#define FLF_UTIL_H

#include <optional>

template <typename V>
using maybe = std::optional<V>;

using nothing = std::bad_optional_access;

#endif
