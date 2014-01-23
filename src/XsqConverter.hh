#ifndef XSQ_CONVERTER
#define XSQ_CONVERTER

// (C) Copyright 2013-2014 INRA
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/optional.hpp>
#include "XsqFile.hh"



class XsqConverter
{
public:
	auto convert(const fs::path&, const fs::path&, const boost::optional<std::vector<std::string>>&) -> void;
private:
	auto convert_reads(const Xsq::Reads&, std::ofstream&, std::ofstream&, const std::string&, const Xsq::YxLocation&, const std::string&, char) -> void;
	static const char cs_map[256];
	static const char* qv_map[256];
	static const std::string QUAL_FILE_EXT;
	static const std::string CSFASTA_FILE_EXT;
	static const unsigned BUFFERS_SIZE;
};

#endif // XSQ_CONVERTER