// (C) Copyright 2013-2014 INRA
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "Parameters.hh"
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <iostream>
#include <stdexcept>

/**
* Function which checks and retrieves command line parameters.
**/
Parameters get_parameters(int argc, char *argv[]) 
{
	Parameters pars;

	// Create options decription
	po::options_description opts_desc(
		"xsq-convert converts XSQ files to csfasta and qval files.\nAvailable options");

	opts_desc.add_options()
		("help,h", "print this help message")
		("input-file,i", po::value<std::string>(&pars.input_file)->required(), "xsq file to convert")
		("output-dir,o", po::value<std::string>(&pars.output_dir)->default_value("."), "directory in which csfasta and qval files are saved")
		("extract-only", po::value<std::vector<std::string> >()->multitoken(), "extract only libraries whose name is prefixed by one of the args. (case sensitive)") 
		;

	// Retrieve and parse command line parameters
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, opts_desc), vm);

	// Print help
	if (argc == 1 || vm.count("help"))
	{
		std::cout << opts_desc << std::endl;
		std::exit(0);
	}

	po::notify(vm);

	// Check input file
	if (!fs::exists(pars.input_file))
	{
		std::cerr << "error: file " << pars.input_file <<  " not found." << std::endl;
		std::exit(-1);
	}

	if (!fs::is_regular_file(pars.input_file))
	{
		std::cerr << "error: " << pars.input_file <<  " is not a regular file." << std::endl;
		std::exit(-1);
	}

	if (fs::extension(pars.input_file) != ".xsq")
	{
		std::cerr << "error: " << pars.input_file << " should have the extension \".xsq\"." << std::endl;
		std::exit(-1);
	}

	// Check output dir
	if (!fs::exists(pars.output_dir))
	{
		std::cerr << "error: directory " << pars.output_dir <<  " not found." << std::endl;
		std::exit(-1);
	}
	if (!fs::is_directory(pars.output_dir))
	{
		std::cerr << "error: " << pars.output_dir <<  " is not a directory." << std::endl;
		std::exit(-1);
	}

	// No way to easily retrieve prefixes_wanted...
	if (vm.count("extract-only"))
		pars.prefixes_wanted = vm["extract-only"].as<std::vector<std::string> >();

	return pars;
}


