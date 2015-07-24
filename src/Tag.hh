#ifndef TAG_HH
#define TAG_HH

// (C) Copyright 2013-2015 INRA
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

		const std::string&  get_name() const
		{
			return m_name;
		}

		char get_start_nucleotide() const
		{
			return s_tags_start.find(m_name)->second;
		}

		Reads get_reads() const;
	};
}
#endif // TAG_HH