#include "common.hpp"

namespace ycel
{
    std::default_random_engine Random::m_Generator(std::random_device{}());
}