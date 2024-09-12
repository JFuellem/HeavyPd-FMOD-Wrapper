//
//  HeavyPdWrapper.cpp
//  HeavyPD-FMOD-Wrapper
//
//  Created by Jonas Füllemann on 09.09.2024.
//

#include "HeavyPdWrapper.hpp"

HeavyPdWrapper::HeavyPdWrapper()
{
    //patchInstance = hv_Sine440_new(sr);
}

void HeavyPdWrapper::Init(int sr)
{
    context = std::make_unique<DYN_TYPE>(sr);
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
