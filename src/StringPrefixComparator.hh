#ifndef STRING_PREFIX__COMPARATOR
#define STRING_PREFIX__COMPARATOR

// (C) Copyright 2013-2014 INRA
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include <string>
#include <boost/algorithm/string/predicate.hpp>

/**
 * Functor used to check if a string is prefixed with another one
 */
class StringPrefixComparator : std::unary_function<std::string, bool>
{
private:
	const std::string m_s;
public:
	StringPrefixComparator(const std::string& s)
		:  m_s(s){};

	auto operator()(const std::string& prefix) const -> bool
	{
		return boost::starts_with(m_s, prefix);
	}
};
#endif // STRING_PREFIX__COMPARATOR