#include "literal.h"
#include "tools.h"

using tool::match;

std::ostream& operator<<(std::ostream& out, const Literal& v)
{
	match<void>(v,
		[&](double d) { out << d; },
		[&](const std::string& str) { out << str; },
		[&](std::nullptr_t) { out << "nullptr"; },
		[&](bool b) { out << (b ? "true" : "false"); }
	);
	return out;
}
