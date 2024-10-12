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

namespace RNBOFMODHelpers {

bool CheckIfOutputQuiet(float* outarray,size_t buffsize, size_t numchans)
{
    buffsize *= numchans;
    for (size_t i = 0; i< buffsize;i++)
    {
        if (abs(outarray[i]) > 1e-9)
            return false;
    }
    return true;
}

FMOD_SPEAKERMODE GetSpeakermode(const int channels)
{
    switch(channels)
    {
        case 1:
            return FMOD_SPEAKERMODE_MONO;
            break;
        case 2:
            return FMOD_SPEAKERMODE_STEREO;
            break;
        case 4:
            return FMOD_SPEAKERMODE_QUAD;
            break;
        case 5:
            return FMOD_SPEAKERMODE_SURROUND;
            break;
        case 6:
            return FMOD_SPEAKERMODE_5POINT1;
            break;
        case 8:
            return FMOD_SPEAKERMODE_7POINT1;
            break;
        case 12:
            return FMOD_SPEAKERMODE_7POINT1POINT4;
            break;
        default:
            return FMOD_SPEAKERMODE_DEFAULT;
            break;
    }
}

void Dispatch3DAttributes(DYN_HV_CONTEXT *obj, void* rawData, uint32_t attributeMap)
{
    FMOD_DSP_PARAMETER_3DATTRIBUTES* param = (FMOD_DSP_PARAMETER_3DATTRIBUTES*)rawData;
    
    if(attributeMap & (1 << AttributeTypes::rel_pos))
    {
        obj->sendMessageToReceiverV(0xfa2454f1, 0, "fff",
                                    param->relative.position.x,
                                    param->relative.position.y,
                                    param->relative.position.z);
    }
    if(attributeMap & (1 << AttributeTypes::rel_vel))
    {
        obj->sendMessageToReceiverV(0xe114e737, 0, "fff",
                                    param->relative.velocity.x,
                                    param->relative.velocity.y,
                                    param->relative.velocity.z);
    }
    if(attributeMap & (1 << AttributeTypes::rel_forw))
    {
        obj->sendMessageToReceiverV(0x6de62bbe, 0, "fff",
                                    param->relative.forward.x,
                                    param->relative.forward.y,
                                    param->relative.forward.z);
    }
    if(attributeMap & (1 << AttributeTypes::rel_up))
    {
        obj->sendMessageToReceiverV(0x385fa13d, 0, "fff",
                                    param->relative.up.x,
                                    param->relative.up.y,
                                    param->relative.up.z);
    }
    if(attributeMap & (1 << AttributeTypes::abs_pos))
    {
        obj->sendMessageToReceiverV(0x6e9bd486, 0, "fff",
                                    param->absolute.position.x,
                                    param->absolute.position.y,
                                    param->absolute.position.z);
    }
    if(attributeMap & (1 << AttributeTypes::abs_vel))
    {
        obj->sendMessageToReceiverV(0xf288817b, 0, "fff",
                                    param->absolute.velocity.x,
                                    param->absolute.velocity.y,
                                    param->absolute.velocity.z);
    }
    if(attributeMap & (1 << AttributeTypes::abs_forw))
    {
        obj->sendMessageToReceiverV(0xc8762842, 0, "fff",
                                    param->absolute.forward.x,
                                    param->absolute.forward.y,
                                    param->absolute.forward.z);
    }
    if(attributeMap & (1 << AttributeTypes::abs_up))
    {
        obj->sendMessageToReceiverV(0x17861e1f, 0, "fff",
                                    param->absolute.up.x,
                                    param->absolute.up.y,
                                    param->absolute.up.z);
    }
}

}
