#include "triangle.hpp"

#include "common.hpp"

namespace ycel
{
	Triangle::Triangle()
	{
	}

	Triangle::Triangle(std::array<hmm_vec2, 3> positions, hmm_vec4 color)
		: m_Positions(positions), m_Color(color)
	{
	}
}
