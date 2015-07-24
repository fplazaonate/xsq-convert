// (C) Copyright 2013-2015 INRA
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "XsqFile.hh"
#include <boost/assign/list_of.hpp>
using namespace Xsq;

/**
* Method which returns a list of all the libraries in this XsqFile
* except those which are unclassified or unassigned.
*/
std::vector<Library> XsqFile::get_libraries() const
{
	std::vector<Library> libraries;

	for (unsigned library_id = 0; library_id < m_file.getNumObjs(); library_id++)
	{
		const std::string& library_name = m_file.getObjnameByIdx(library_id);

		if (std::find_if(s_reserved_names.begin(), s_reserved_names.end(), StringPrefixComparator(library_name)) == s_reserved_names.end())
			libraries.push_back(Library(library_name, m_file.openGroup(library_name)));
	}
		
	return libraries;
}

/**
* Method which returns a list of all the libraries in this XsqFile
* whose name is prefixed by one of the elements in the prefixes_wanted list.
*/
std::vector<Library> XsqFile::get_libraries_by_prefix(const std::vector<std::string>& prefixes_wanted) const
{
	std::vector<Library> libraries;

	for (unsigned library_id = 0; library_id < m_file.getNumObjs(); library_id++)
	{
		const std::string& library_name = m_file.getObjnameByIdx(library_id);

		if (std::find_if(prefixes_wanted.begin(), prefixes_wanted.end(), StringPrefixComparator(library_name)) != prefixes_wanted.end())
		{
			libraries.push_back(Library(library_name, m_file.openGroup(library_name)));
		}
	}

	return libraries;
}


/**
* Method which returns a vector of all the tags used in this XsqFile.
* e.g: 'F3', 'R3'
*/
std::vector<std::string> XsqFile::get_used_tags_names() const
{
	const H5::Group& run_metadata_group = m_file.openGroup("RunMetadata");
	const H5::Group& tag_details_group = run_metadata_group.openGroup("TagDetails");

	hsize_t nb_tags = tag_details_group.getNumObjs(); 
	std::vector<std::string> used_tags_names(nb_tags);

	for (unsigned tag_id = 0; tag_id < nb_tags; tag_id++)
	{
		std::string tag_name = tag_details_group.getObjnameByIdx(tag_id);
		used_tags_names[tag_id] = tag_name;
	}

	return used_tags_names;
}

/**
* Groups which are not libraries or libraries which should not be extracted
*/
const std::vector<std::string> XsqFile::s_reserved_names =
	boost::assign::list_of( "RunMetadata")("TagDetails")("Indexing")("Unassigned")("Unclassified");
