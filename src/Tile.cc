// (C) Copyright 2013-2014 INRA
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "Tile.hh"
using namespace Xsq;

std::list<Tag> Tile::get_tags() const
{
	std::list<Tag> tags;

	for (unsigned tag_id = 0; tag_id < m_group.getNumObjs(); tag_id++)
	{
		const std::string& tile_name = m_group.getObjnameByIdx(tag_id);

		if (tile_name != s_reserved_name)
			tags.push_back(Tag(tile_name, m_group.openGroup(tile_name)));
	}
	
	return tags;
}

YxLocation  Tile::get_yxLocation() const
{
	const H5::Group& fragments_group = m_group.openGroup("Fragments");
	const H5::DataSet& yxLocation_ds = fragments_group.openDataSet("yxLocation");
	
	return YxLocation(yxLocation_ds);
}

const std::string Tile::s_reserved_name = "Fragments";
