#ifndef MPP_GENERATORS_LOOP_HPP
#define MPP_GENERATORS_LOOP_HPP

#include "section.hpp"
#include "project_config.hpp"

namespace mpp
{
    template <typename Input>
    using Loop = Section<Input, SAMPLES_SIZE>;
}

#endif
