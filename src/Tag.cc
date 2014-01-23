// (C) Copyright 2013-2014 INRA
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "Tag.hh"
using namespace Xsq;

auto Tag::get_reads() const -> Reads
{
	H5::DataSet color_call_qv_ds = m_group.openDataSet("ColorCallQV");
	return Reads(color_call_qv_ds);
}

const std::map<std::string, char> Tag::s_tags_start =
	{{"F3", 'T'},{"F5-DNA", 'T'},{"R3", 'G'}};
