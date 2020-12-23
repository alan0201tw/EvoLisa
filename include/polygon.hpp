#pragma once

#include <array>

#include "HandmadeMath.h"

namespace ycel
{
	template<uint32_t V>
	class Polygon
	{
	public:
		Polygon() {}
		Polygon(std::array<hmm_vec2, V> positions, hmm_vec4 color)
			: m_Positions(positions), m_Color(color)
		{}

		const std::array<hmm_vec2, V>& GetPositions() const { return m_Positions; }
		const hmm_vec4& GetColor() const { return m_Color; }

		std::array<hmm_vec2, V>& GetPositions() { return m_Positions; }
		hmm_vec4& GetColor() { return m_Color; }

		uint32_t GetVerticesCount() const { return V; }

	private:
		std::array<hmm_vec2, V> m_Positions;
		hmm_vec4 m_Color;
	};
}
