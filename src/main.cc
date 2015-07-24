// (C) Copyright 2013-2015 INRA
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "Parameters.hh"
#include "XsqConverter.hh"
#include <exception>
#include <new>

void no_more_memory()
{
	std::cerr << "error: Memory allocation failed." << std::endl;
	std::exit (1);
}

int main(int argc, char *argv[])
{
	std::set_new_handler(no_more_memory);

	try 
	{	
		Parameters pars = get_parameters(argc, argv);
		XsqConverter().convert(pars.input_file, pars.output_dir, pars.prefixes_wanted);
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
