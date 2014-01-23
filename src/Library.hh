#ifndef LIBRARY_HH
#define LIBRARY_HH

// (C) Copyright 2013-2014 INRA
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "H5Cpp.h"
#include <string>
#include <vector>
#include "Tile.hh"

namespace Xsq 
{
	class Library
	{
	private:
		std::string m_complete_name;
		std::string m_sample_name;
		H5::Group m_group;

	public:
		Library(const std::string& name, const H5::Group& group);

		auto get_complete_name() const -> const std::string&
		{
			return m_complete_name;
		}

		auto get_sample_name() const -> const std::string&
		{
			return m_sample_name;
		}

		auto get_tiles() const -> std::vector<Tile>;
	};
}
#endif // LIBRARY_HH