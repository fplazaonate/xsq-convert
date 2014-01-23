#ifndef TILE_HH
#define TILE_HH

// (C) Copyright 2013 Florian Plaza Oñate
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "H5Cpp.h"
#include "Tag.hh"
#include "YxLocation.hh"
#include <string>
#include <list>
#include <boost/algorithm/string/trim.hpp>


namespace Xsq
{
	class Tile
	{
	private:
		std::string m_name;
		H5::Group m_group;
		static const std::string s_reserved_name;

	public:
		Tile(const std::string& name, const H5::Group& group)
			: m_name(name), m_group(group)
		{
			boost::trim_left_if(m_name, boost::is_any_of("0"));
		};

		auto get_name() const -> const std::string&
		{
			return m_name;
		};

		auto get_tags() const -> std::list<Tag>;

		auto get_yxLocation() const -> YxLocation;
	};
}

#endif // TILE_HH
