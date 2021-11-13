#pragma once

#define __cpp_lib_format

#include <format>
#include <string>
#include <string_view>
#include <vector>
#include <iostream>
#include <algorithm>
#include <array>
#include <memory>

using std::make_unique;
using std::unique_ptr;
using std::vector;

#ifdef NDEBUG
#define ASSERT(_) (void(0))
#else
#include <cassert>
#define ASSERT(x) assert(x)
#endif
