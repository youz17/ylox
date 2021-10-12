#pragma once

#include <variant>
#include <iostream>

using Literal = std::variant<double, bool, std::nullptr_t, std::string>;

std::ostream& operator<<(std::ostream& out, const Literal& v);
