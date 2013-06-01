#ifndef XSQ_FILE_HH
#define XSQ_FILE_HH

// (C) Copyright 2013 Florian Plaza Oñate
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "H5Cpp.h"
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <vector>
#include <list>
#include "Library.hh"
#include "StringPrefixComparator.hh"



namespace Xsq
{
	class XsqFile
	{
	private:
		H5::H5File m_file;
		fs::path m_path;
		static const std::vector<std::string> s_reserved_names;

	public:
		XsqFile(const std::string& path)
			: m_file(path, H5F_ACC_RDONLY), m_path(path){};

		XsqFile()
		{}

		~XsqFile()
		{
			m_file.close();
		};

		auto get_path() const-> fs::path
		{
			return m_path;
		}

		auto get_libraries() const -> std::vector<Library>;

		auto get_libraries_by_prefix(const std::vector<std::string>&) const -> std::vector<Library>;

		auto get_used_tags_names() const -> std::vector<std::string>;
	};
}

#endif // XSQ_FILE_HH