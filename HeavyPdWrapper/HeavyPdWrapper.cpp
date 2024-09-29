//
//  HeavyPdWrapper.cpp
//  HeavyPD-FMOD-Wrapper
//
//  Created by Jonas Füllemann on 09.09.2024.
//

#include "HeavyPdWrapper.hpp"

HeavyPdWrapper::HeavyPdWrapper()
{
}

void HeavyPdWrapper::Init()
{
    size_t c(0);
    do
    {
        context.push_back(std::make_unique<DYN_HV_CONTEXT>(sampleRate));
        c++;
    } while (multiChannelExpandable && c<MAX_CHANS);
}

const std::unordered_map<const char*, FMODHelperMethods::AttributeTypes> FMODHelperMethods::stringToAttribute = {
    {"rel_pos", rel_pos},
    {"rel_vel", rel_vel},
    {"rel_forw", rel_forw},
    {"rel_up", rel_up},
    {"abs_pos", abs_pos},
    {"abs_vel", abs_vel},
    {"abs_forw", abs_forw},
    {"abs_up", abs_up}
};
