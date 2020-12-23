#include "image.hpp"

namespace ycel
{
	Image::Image(const std::string& path)
	{
		stbi_set_flip_vertically_on_load_thread(1);

		int width, height, channels;
		// Here we take in 3 channels only, namely RGB channels
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 3);

		m_Width = width;
		m_Height = height;
		m_Data = data;
	}

	Image::~Image()
	{
		stbi_image_free(m_Data);
	}

	uint32_t Image::PixelDifferenceMSE(u_char* reference) const
	{
		uint32_t result = 0;
		for (uint32_t i = 0; i < m_Width * m_Height * 3; ++i)
		{
			int offset = m_Data[i] - reference[i];
			result += offset * offset;
		}

		return result;
	}
}
