#ifndef READS_HH
#define READS_HH

// (C) Copyright 2013-2014 INRA
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include <cstdint>

namespace Xsq
{
	class Reads
	{
	private:
		uint8_t* m_data;
		unsigned m_nb_reads;
		unsigned m_reads_length;

	public:
		Reads(const H5::DataSet& reads_ds)
		{
			hsize_t color_call_qv_ds_dims[2];
			reads_ds.getSpace().getSimpleExtentDims(
				color_call_qv_ds_dims, NULL);

			m_nb_reads = color_call_qv_ds_dims[0];
			m_reads_length = color_call_qv_ds_dims[1];

			m_data = new uint8_t[m_nb_reads*m_reads_length];
			reads_ds.read(
				 m_data, H5::PredType::NATIVE_UINT8);
		};

		~Reads()
		{
			delete [] m_data;
		}

		uint8_t* get_read(unsigned read_id) const
		{
			return m_data + read_id*m_reads_length;
		}

		unsigned get_nb_reads() const
		{
			return m_nb_reads;
		}

		unsigned get_reads_length() const
		{
			return m_reads_length;
		}
	};
}

#endif // READS_HH