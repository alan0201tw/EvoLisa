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

		void SortVertices()
		{
			hmm_vec2 center;
			center[0] = center[1] = 0.0f;
			for (uint32_t vid = 0; vid < V; ++vid)
			{
				center.X += m_Positions[vid].X;
				center.Y += m_Positions[vid].Y;
			};
			center /= V;

			std::sort(m_Positions.begin(), m_Positions.end(), 
			[center](const hmm_vec2& a, const hmm_vec2& b) -> bool
			{
				float aa = std::atan2(a.Y - center.Y, a.X - center.X);
				float bb = std::atan2(b.Y - center.Y, b.X - center.X);
				return aa < bb;
			});
		}

	private:
		std::array<hmm_vec2, V> m_Positions;
		hmm_vec4 m_Color;
	};
}
