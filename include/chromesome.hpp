#pragma once

#include <cstdint>
#include <array>
#include <algorithm>
#include <cmath>

#include "common.hpp"
#include "polygon.hpp"

namespace ycel
{
	// One Chromesome represents a certain state of the canvas, 
	// meaning it should contain multiple primitives like triangles or polygons.

	enum class CrossoverType
	{
		FullRandom,
		Devision
	};

	template<uint32_t V, uint32_t T>
	class Chromesome
	{
	public:
		// Constructor with no arguments will be initialized randomly
		Chromesome()
		{
			for (uint32_t i = 0; i < T; ++i)
			{
				// create a convex polygon
				std::array<hmm_vec2, V> positions;
				for (uint32_t vid = 0; vid < V; ++vid)
				{
					positions[vid].X = Random::RandomAbs1();
					positions[vid].Y = Random::RandomAbs1();
				};

				hmm_vec4 color;
				color.R = Random::Random01();
				color.G = Random::Random01();
				color.B = Random::Random01();
				//color.A = Random::Random01();
				color.A = 0.15f;

				m_Primitives[i] = Polygon<V>(positions, color);
			}

			m_Fitness = 0;
		}

		Chromesome(const Chromesome& other)
		{
			m_Primitives = other.GetPrimitives();
			m_Fitness = other.m_Fitness;
		}

		Chromesome(Chromesome&& other)
		{
			std::swap(m_Primitives, other.m_Primitives);
			std::swap(m_Fitness, other.m_Fitness);
		}

		Chromesome& operator=(const Chromesome& other)
		{
			m_Primitives = other.GetPrimitives();
			m_Fitness = other.m_Fitness;

			return *this;
		}

		inline bool operator < (const Chromesome& p) const
		{
			return m_Fitness < p.m_Fitness;
		}

		uint32_t GetFitness() { return m_Fitness; }
		void UpdateFitness(uint32_t fitness) { m_Fitness = fitness; }

		const std::array<Polygon<V>, T>& GetPrimitives() const { return m_Primitives; }

		static Chromesome Crossover(
			const Chromesome& p0, const Chromesome& p1, CrossoverType type = CrossoverType::Devision)
		{
			Chromesome child;

			if (type == CrossoverType::Devision)
			{
				uint32_t crossoverPoint = static_cast<uint32_t>(Random::RandomInt(0, T));

				for (uint32_t i = 0; i < crossoverPoint; ++i)
					child.m_Primitives[i] = p0.GetPrimitives()[i];
				for (uint32_t i = crossoverPoint; i < T; ++i)
					child.m_Primitives[i] = p1.GetPrimitives()[i];
			}
			else
			{
				for (uint32_t i = 0; i < T; ++i)
				{
					if (Random::Random01() < 0.5f)
						child.m_Primitives[i] = p0.GetPrimitives()[i];
					else
						child.m_Primitives[i] = p1.GetPrimitives()[i];
				}
			}

			return child;
		}

		static void Mutate(Chromesome& chromesome)
		{
			for (uint32_t i = 0; i < T; ++i)
			{
				// iterate over V vertices
				for (uint32_t vid = 0; vid < V; ++vid)
				{
					// X and Y
					for (uint32_t dim = 0; dim < 2; ++dim)
					{
						if (Random::Random01() < 0.75f) continue;

						float posPert = 0.02f * Random::RandomAbs1();

						chromesome.m_Primitives[i].GetPositions()[vid][dim] += posPert;
						float& val = chromesome.m_Primitives[i].GetPositions()[vid][dim];
						if (val < -1.0f || val > 1.0f) val = Random::RandomAbs1();
					}
				}

				// set dim < 3 to ignore alpha
				for (uint32_t dim = 0; dim < 3; ++dim)
				{
					if (Random::Random01() < 0.5f) continue;

					float colPert = 0.02f * Random::RandomAbs1();
					chromesome.m_Primitives[i].GetColor()[dim] += colPert;
					float& val = chromesome.m_Primitives[i].GetColor()[dim];
					if (val < -1.0f || val > 1.0f) val = Random::Random01();
				}
			}
		}

		static void MutateRand(Chromesome& chromesome)
		{
			for (uint32_t i = 0; i < T; ++i)
			{
				// iterate over V vertices
				for (uint32_t vid = 0; vid < V; ++vid)
				{
					// X and Y
					for (uint32_t dim = 0; dim < 2; ++dim)
					{
						if (Random::Random01() < 0.5f) continue;

						chromesome.m_Primitives[i].GetPositions()[vid][dim] = Random::RandomAbs1();
					}
				}

				// set dim < 3 to ignore alpha
				for (uint32_t dim = 0; dim < 3; ++dim)
				{
					if (Random::Random01() < 0.5f) continue;

					chromesome.m_Primitives[i].GetColor()[dim] = Random::Random01();
				}
			}
		}

	private:
		std::array<Polygon<V>, T> m_Primitives;
		uint32_t m_Fitness;
	};
}
