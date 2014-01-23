#ifndef TAG_HH
#define TAG_HH

// (C) Copyright 2013 Florian Plaza Oñate
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "H5Cpp.h"
#include <string>
#include <map>
#include "Reads.hh"

namespace Xsq
{
	class Tag
	{
	private:
		std::string m_name;
		H5::Group m_group;
		static const std::map<std::string, char> s_tags_start;
		

	public:
		Tag(const std::string& name, const H5::Group& group)
			: m_name(name), m_group(group){};

		auto get_name() const -> const std::string& 
		{
			return m_name;
		}

		auto get_start_nucleotide() const -> char
		{
			return s_tags_start.find(m_name)->second;
		}

		auto get_reads() const -> Reads;
	};
}
#endif // TAG_HH