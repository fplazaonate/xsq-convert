// (C) Copyright 2013 Florian Plaza Oñate
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "XsqConverter.hh"
#include <memory>

auto XsqConverter::convert(const fs::path& input_file, const fs::path& output_dir, const boost::optional<std::vector<std::string>>& prefixes_wanted) -> void
{
	Xsq::XsqFile file(input_file.string());
	const auto& used_tags_names = file.get_used_tags_names();

	std::map<std::string, char*> qual_ofs_buffers;
	std::map<std::string, char*> csfasta_ofs_buffers;

	// Create buffers for latter use in qual and csfasta ofstreams
	for(const auto& tag_name: used_tags_names)
	{
		qual_ofs_buffers[tag_name] = new char[BUFFERS_SIZE];
		csfasta_ofs_buffers[tag_name] = new char[BUFFERS_SIZE];
	}

	const auto& libraries = prefixes_wanted ? 
		file.get_libraries_by_prefix(*prefixes_wanted) : file.get_libraries();

	for(const auto& library: libraries)
	{
		// Create the library output directory
		const fs::path& library_output_dir
			= output_dir / fs::path(library.get_sample_name());

		fs::create_directory(library_output_dir);

		// Create and open csfasta and qval output files
		std::map<std::string, std::unique_ptr<std::ofstream>> qual_ofs;
		std::map<std::string, std::unique_ptr< std::ofstream>> csfasta_ofs;
		
		for(const auto& tag_name: used_tags_names)
		{
			const std::string& library_output_filename = 
				file.get_path().stem().string() + '_' + library.get_complete_name() + '_' + tag_name;

			const fs::path& output_qual_file_path =
				library_output_dir / fs::path(library_output_filename + QUAL_FILE_EXT);
	
			const fs::path& output_csfasta_file_path =
				library_output_dir / fs::path(library_output_filename + CSFASTA_FILE_EXT);

			// Create streams
			qual_ofs[tag_name] = std::unique_ptr<std::ofstream>(new std::ofstream());
			qual_ofs[tag_name]->open(output_qual_file_path.string().c_str());
			csfasta_ofs[tag_name] = std::unique_ptr<std::ofstream>(new std::ofstream());
			csfasta_ofs[tag_name]->open(output_csfasta_file_path.string().c_str());

			// Make streams use buffers
			qual_ofs[tag_name]->rdbuf()->pubsetbuf(qual_ofs_buffers[tag_name], BUFFERS_SIZE);
			qual_ofs[tag_name]->rdbuf()->pubsetbuf(csfasta_ofs_buffers[tag_name], BUFFERS_SIZE);
		}


		std::cout << "info: Extracting library " << library.get_complete_name() << ". Please wait..." << std::endl;

		// Extract all reads of the library in csfasta and qval files
		for(const auto& tile: library.get_tiles())
		{
			const std::string& tile_name = tile.get_name();
	
			const Xsq::YxLocation& tile_yxLocation = tile.get_yxLocation();

			for(const auto& tag: tile.get_tags())
			{
				const std::string& tag_name = tag.get_name();
				convert_reads(
					tag.get_reads(), *qual_ofs[tag_name], *csfasta_ofs[tag_name], tile_name, tile_yxLocation, tag_name, tag.get_start_nucleotide());
			}
		}

		for(const auto& tag_name: used_tags_names)
		{
			qual_ofs[tag_name]->close();
			csfasta_ofs[tag_name]->close();
		}

	}

	for(const auto& tag_name: used_tags_names)
	{
		delete [] qual_ofs_buffers[tag_name];
		delete [] csfasta_ofs_buffers[tag_name];
	}

}

auto XsqConverter::convert_reads(const Xsq::Reads& reads, std::ofstream& qual_ofs, std::ofstream& csfasta_ofs, const std::string& tile_name, const Xsq::YxLocation& yxLocation, const std::string& tag_name, char start_nucleotide) -> void
{
	unsigned nb_reads = reads.get_nb_reads();
	unsigned reads_length = reads.get_reads_length();
				
	for (unsigned read_id = 0; read_id < nb_reads; read_id++)
	{
		const auto& location =
			yxLocation.get_location(read_id);
		const std::string& read_header =
			'>' + tile_name + '_' +
			std::to_string(location.first) + '_' + std::to_string(location.second) + '_' +
			tag_name + '\n';

		qual_ofs << read_header;
		csfasta_ofs << read_header;
		csfasta_ofs << start_nucleotide;

		// Hackish handmade loop unrolling.
		// 3-4% performance boost on big xsq files
		uint8_t* read_data = reads.get_read(read_id);
		unsigned i;
		for (i = 0; i <= reads_length-4; i += 4)
		{
			unsigned values = *(unsigned*)(read_data+i);
			uint8_t value[4];
			value[0] = values & 0xff;
			value[1] = (values >> 8) & 0xff;
			value[2] = (values >> 16) & 0xff;
			value[3] = (values >> 24) & 0xff;
			
			qual_ofs << qv_map[value[0]] 
				<< qv_map[value[1]] 
				<< qv_map[value[2]]
				<< qv_map[value[3]];

			csfasta_ofs << cs_map[value[0]]
				<< cs_map[value[1]]
				<< cs_map[value[2]]
				<< cs_map[value[3]];
		}

		for (; i < reads_length; i++)
		{
			uint8_t value = read_data[i];

			qual_ofs << qv_map[value];
			csfasta_ofs << cs_map[value];
		}

		qual_ofs << '\n';
		csfasta_ofs << '\n';
	}
}

const std::string XsqConverter::QUAL_FILE_EXT = ".QV.qual";
const std::string XsqConverter::CSFASTA_FILE_EXT = ".csfasta";

// Map CallQV -> qv
// 
const char* XsqConverter::qv_map[256] = {
	"0 ","0 ","0 ","0 ","1 ","1 ","1 ","1 ","2 ","2 ","2 ","2 ","3 ","3 ","3 ","3 ",
	"4 ","4 ","4 ","4 ","5 ","5 ","5 ","5 ","6 ","6 ","6 ","6 ","7 ","7 ","7 ","7 ",
	"8 ","8 ","8 ","8 ","9 ","9 ","9 ","9 ","10 ","10 ","10 ","10 ","11 ","11 ","11 ","11 ",
	"12 ","12 ","12 ","12 ","13 ","13 ","13 ","13 ","14 ","14 ","14 ","14 ","15 ","15 ","15 ","15 ",
	"16 ","16 ","16 ","16 ","17 ","17 ","17 ","17 ","18 ","18 ","18 ","18 ","19 ","19 ","19 ","19 ",
	"20 ","20 ","20 ","20 ","21 ","21 ","21 ","21 ","22 ","22 ","22 ","22 ","23 ","23 ","23 ","23 ",
	"24 ","24 ","24 ","24 ","25 ","25 ","25 ","25 ","26 ","26 ","26 ","26 ","27 ","27 ","27 ","27 ",
	"28 ","28 ","28 ","28 ","29 ","29 ","29 ","29 ","30 ","30 ","30 ","30 ","31 ","31 ","31 ","31 ",
	"32 ","32 ","32 ","32 ","33 ","33 ","33 ","33 ","34 ","34 ","34 ","34 ","35 ","35 ","35 ","35 ",
	"36 ","36 ","36 ","36 ","37 ","37 ","37 ","37 ","38 ","38 ","38 ","38 ","39 ","39 ","39 ","39 ",
	"40 ","40 ","40 ","40 ","41 ","41 ","41 ","41 ","42 ","42 ","42 ","42 ","43 ","43 ","43 ","43 ",
	"44 ","44 ","44 ","44 ","45 ","45 ","45 ","45 ","46 ","46 ","46 ","46 ","47 ","47 ","47 ","47 ",
	"48 ","48 ","48 ","48 ","49 ","49 ","49 ","49 ","50 ","50 ","50 ","50 ","51 ","51 ","51 ","51 ",
	"52 ","52 ","52 ","52 ","53 ","53 ","53 ","53 ","54 ","54 ","54 ","54 ","55 ","55 ","55 ","55 ",
	"56 ","56 ","56 ","56 ","57 ","57 ","57 ","57 ","58 ","58 ","58 ","58 ","59 ","59 ","59 ","59 ",
	"60 ","60 ","60 ","60 ","61 ","61 ","61 ","61 ","62 ","62 ","62 ","62 ","63 ","63 ","63 ","63 "
};

const char XsqConverter::cs_map[256] = {
	'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '0', '1', '2', '3',
	'0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3',
	'0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3',
	'0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3',
	'0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3',
	'0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3',
	'0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3',
	'0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3',
	'0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3',
	'0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3',
	'0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3',
	'0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3',
	'0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3',
	'0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3',
	'0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3',
	'0', '1', '2', '3', '0', '1', '2', '3', '0', '1', '2', '3', '.', '.', '.', '.'
};

// 1 MB
const unsigned XsqConverter::BUFFERS_SIZE = 1 << 20;