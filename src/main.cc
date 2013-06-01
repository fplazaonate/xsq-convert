// (C) Copyright 2013 Florian Plaza Oñate
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "XsqConverter.hh"
#include <cstdlib>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <exception>
#include <new>

auto no_more_memory() -> void
{
	std::cerr << "error: Memory allocation failed." << std::endl;
	std::exit (1);
}

/**
* Function which checks and retrieves command line parameters.
**/
auto get_parameters(int argc, char *argv[], fs::path& input_file, fs::path& output_dir, boost::optional<std::vector<std::string> >& prefixes_wanted) -> void
{
	// Create options decription
	po::options_description opts_desc(
		"xsq-convert converts XSQ files to csfasta and qval files.\nAvailable options");

	opts_desc.add_options()
		("help,h", "print this help message")
		("input-file,i", po::value<std::string>(), "xsq file to convert")
		("output-dir,o", po::value<std::string>()->default_value("."), "directory in which cfasta and qval files are saved")
		("extract-only", po::value<std::vector<std::string> >()->multitoken(), "extract only libraries whose name is prefixed by one of the args. (case sensitive)") 
		;

	// Retrieve and parse command line parameters
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, opts_desc), vm);
	po::notify(vm);

	// Print help
	if (vm.size() == 1 || vm.count("help"))
	{
		std::cout << opts_desc << std::endl;
		std::exit(0);
	}

	// Check input file
	if (!vm.count("input-file"))
	{
		std::cerr << "error: No XSQ input file. Goodbye." << std::endl;
		std::exit(-1);
	}
	else
	{
		input_file = vm["input-file"].as<std::string>();

		if (!fs::exists(input_file))
		{
			std::cerr << "error: file " << input_file <<  " not found." << std::endl;
			std::exit(-1);
		}

		if (!fs::is_regular_file(input_file))
		{
			std::cerr << "error: " << input_file <<  " is not a regular file." << std::endl;
			std::exit(-1);
		}

		if (fs::extension(input_file) != ".xsq")
		{
			std::cerr << "error: " << input_file << " should have the extension \".xsq\"." << std::endl;
			std::exit(-1);
		}
	}

	// Check output dir
	output_dir = vm["output-dir"].as<std::string>();
	if (!fs::exists(output_dir))
	{
		std::cerr << "error: directory " << output_dir <<  " not found." << std::endl;
		std::exit(-1);
	}
	if (!fs::is_directory(output_dir))
	{
		std::cerr << "error: " << output_dir <<  " is not a directory." << std::endl;
		std::exit(-1);
	}

	if (vm.count("extract-only"))
		prefixes_wanted = vm["extract-only"].as<std::vector<std::string> >();

}

auto main(int argc, char *argv[]) -> int
{
	std::set_new_handler(no_more_memory);
	Xsq::XsqFile f;
	fs::path input_file;
	fs::path output_dir;
	boost::optional<std::vector<std::string> > prefixes_wanted;

	try 
	{	
		get_parameters(argc, argv, input_file, output_dir, prefixes_wanted);
		XsqConverter().convert(input_file, output_dir, prefixes_wanted);
		std::exit(0);
	}
	catch (const H5::Exception& h5e)
	{
		std::cerr << h5e.getDetailMsg() << std::endl;
		std::exit(-1);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::exit(-1);
	}
}