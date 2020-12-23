#pragma once

#include <random>
#include <chrono>

#include <iostream>

namespace ycel
{
    class Random
    {
    public:
    
        static int RandomInt(int min, int max)
        {
            std::uniform_int_distribution<int> distribution(min, max);

            return distribution(m_Generator);
        };

        static float Random01()
        {
            std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

            return distribution(m_Generator);
        };

        static float RandomAbs1()
        {
            std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

            return distribution(m_Generator);
        };

        static float RandomRange(float min, float max)
        {
            return min + Random01() * (max - min);
        }

    private:
        static std::default_random_engine m_Generator;
    };

}