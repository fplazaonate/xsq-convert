#ifndef YX_LOCATION_HH
#define YX_LOCATION_HH

// (C) Copyright 2013 Florian Plaza Oñate
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include <cstdint>
#include <utility>

namespace Xsq
{
	class YxLocation
	{
	private:
		uint16_t* m_data;
		unsigned m_nb_locations;

	public:
		YxLocation(const H5::DataSet& yxLocation_ds)
		{
			hsize_t yxLocation_ds_dims[2];
			yxLocation_ds.getSpace().getSimpleExtentDims(
				yxLocation_ds_dims, NULL);

			m_nb_locations = yxLocation_ds_dims[0];

			m_data = new uint16_t[m_nb_locations*2];
			yxLocation_ds.read(
				m_data, H5::PredType::NATIVE_UINT16);
		};

		~YxLocation()
		{
			delete [] m_data;
		}

		auto get_location(unsigned i) const -> std::pair<uint16_t, uint16_t>
		{
			return std::make_pair(m_data[i*2], m_data[i*2+1]);
		}

		auto get_nb_locations() const -> unsigned
		{
			return m_nb_locations;
		}
	};
}

#endif // YX_LOCATION_HH