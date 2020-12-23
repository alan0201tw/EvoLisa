#pragma once

#include <array>

#include "HandmadeMath.h"

namespace ycel
{
	class Triangle
	{
	public:
		Triangle();
		Triangle(std::array<hmm_vec2, 3> positions, hmm_vec4 color);

		const std::array<hmm_vec2, 3>& GetPositions() const { return m_Positions; }
		const hmm_vec4& GetColor() const { return m_Color; }

		std::array<hmm_vec2, 3>& GetPositions() { return m_Positions; }
		hmm_vec4& GetColor() { return m_Color; }

	private:
		std::array<hmm_vec2, 3> m_Positions;
		hmm_vec4 m_Color;
	};
}
