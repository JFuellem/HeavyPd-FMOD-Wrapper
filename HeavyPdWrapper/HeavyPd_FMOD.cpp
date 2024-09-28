//
//  HeavyPD_FMOD.hpp
//  HeavyPD-FMOD-Wrapper
//
//  Created by Jonas Füllemann on 09.09.2024.
//

#ifndef HeavyPD_FMOD_hpp
#define HeavyPD_FMOD_hpp

#include <stdio.h>
#include <string.h>
#include "HeavyPdWrapper.hpp"

#define MAX_HEAVYPD_PARAMETERS 1000

extern "C" {
    F_EXPORT FMOD_DSP_DESCRIPTION* F_CALL FMODGetDSPDescription();
}

FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dspcreate (FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dsprelease (FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dspreset (FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dspprocess (FMOD_DSP_STATE *dsp_state, unsigned int length, const FMOD_DSP_BUFFER_ARRAY *inbufferarray, FMOD_DSP_BUFFER_ARRAY *outbufferarray, FMOD_BOOL inputsidle, FMOD_DSP_PROCESS_OPERATION op);
FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dspsetparamfloat (FMOD_DSP_STATE *dsp_state, int index, float value);
FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dspgetparamfloat (FMOD_DSP_STATE *dsp_state, int index, float *value, char *valuestr);
FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dspsetparamdata(FMOD_DSP_STATE *dsp_state, int index, void *data, unsigned int /*length*/);
FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dspgetparamdata(FMOD_DSP_STATE * /*dsp_state*/, int index, void ** /*value*/, unsigned int * /*length*/, char * /*valuestr*/);
FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_sys_register (FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_sys_deregister (FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_sys_mix (FMOD_DSP_STATE *dsp_state, int stage);

static FMOD_DSP_PARAMETER_DESC heavyPdParameters[MAX_HEAVYPD_PARAMETERS];
static bool FMOD_HEAVYPD_Running = false;
struct UserDataStruct
{
    size_t attributesIndex;
    float tailLength;
    bool isInstrument;
    uint32_t attributeBitmap;
    std::map<int, uint32_t> parameterIndexToHash;
    bool multiChannelExtendable;
};
static UserDataStruct userData;
FMOD_DSP_PARAMETER_DESC *FMOD_HEAVYPD_dspparam[MAX_HEAVYPD_PARAMETERS] = {};


FMOD_DSP_DESCRIPTION FMOD_HEAVYPD_Desc =
{
    FMOD_PLUGIN_SDK_VERSION,
    "JRF HEAVYPDWrapper", //name
    0x00010000, //plugin version
    0, //inbuffer
    0, //outbuffer
    FMOD_HEAVYPD_dspcreate,
    FMOD_HEAVYPD_dsprelease,
    FMOD_HEAVYPD_dspreset,
    0,
    FMOD_HEAVYPD_dspprocess,
    0,
    MAX_HEAVYPD_PARAMETERS,
    FMOD_HEAVYPD_dspparam,
    FMOD_HEAVYPD_dspsetparamfloat, //setfloat
    0, //setint
    0, //setbool
    FMOD_HEAVYPD_dspsetparamdata, //setdata
    FMOD_HEAVYPD_dspgetparamfloat, //getfloat
    0, //getint
    0, //getbool
    FMOD_HEAVYPD_dspgetparamdata, //getdata
    0,
    0, //userdata
    FMOD_HEAVYPD_sys_register,
    FMOD_HEAVYPD_sys_deregister,
    FMOD_HEAVYPD_sys_mix
};



extern "C"
{
    F_EXPORT FMOD_DSP_DESCRIPTION* F_CALL FMODGetDSPDescription()
    {
        DYN_TYPE dummyObj(48000);
        
        //get name of plugin
        const char* name = dummyObj.getName();
        strcpy(FMOD_HEAVYPD_Desc.name, name);
        
        //get number of parameters
        int numParameters = dummyObj.getParameterInfo(0, nullptr);
        
        int finalParameters = 0;
        
        bool hasAny3dAttributes = false;
        
        //UserDataStruct *userData = new UserDataStruct();
        
        //get somehow the parameter metadata
        for (int i = 0; i<numParameters;i++)
        {
            HvParameterInfo info;
            dummyObj.getParameterInfo(i, &info);
            
            if(FMODHelperMethods::stringToAttribute.contains(info.name))
            {
                int attributeIndex = FMODHelperMethods::stringToAttribute.at(info.name);
                userData.attributeBitmap |= 1 << attributeIndex;
                hasAny3dAttributes = true;
            }
            else if (std::char_traits<char>::compare(info.name, "Sys_tail", 8) == 0)
            {
                //get tail setting
                userData.tailLength = info.minVal;
            }
            else
            {
                FMOD_HEAVYPD_dspparam[finalParameters] = &heavyPdParameters[finalParameters];
                userData.parameterIndexToHash[finalParameters] = info.hash;
                FMOD_DSP_INIT_PARAMDESC_FLOAT(heavyPdParameters[finalParameters], info.name, "", "HeavyPdParam", info.minVal, info.maxVal, info.defaultVal);
                finalParameters++;
            }
        }
        
        if(hasAny3dAttributes)
        {
            FMOD_HEAVYPD_dspparam[finalParameters] = &heavyPdParameters[finalParameters];
            FMOD_DSP_INIT_PARAMDESC_DATA(heavyPdParameters[finalParameters],"3D Attributes","","", FMOD_DSP_PARAMETER_DATA_TYPE_3DATTRIBUTES);
            
            userData.attributesIndex = finalParameters;
            
            finalParameters++;
        }
        
        //check in - output dimensions -> type
        if(dummyObj.getNumInputChannels() > 0)
        {
            FMOD_HEAVYPD_Desc.numinputbuffers = 1;
            userData.isInstrument = false;
            
            userData.multiChannelExtendable = dummyObj.getNumInputChannels() == 1 && dummyObj.getNumOutputChannels() == 1;
        }
        else
        {
            userData.isInstrument = true;
        }
        
        if(dummyObj.getNumOutputChannels() > 0)
            FMOD_HEAVYPD_Desc.numoutputbuffers = 1;
        
        FMOD_HEAVYPD_Desc.userdata = &userData;
        FMOD_HEAVYPD_Desc.numparameters = finalParameters;
        
        return &FMOD_HEAVYPD_Desc;
    }
}

FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dspcreate(FMOD_DSP_STATE *dsp_state)
{
    dsp_state->plugindata = (HeavyPdWrapper *)FMOD_DSP_ALLOC(dsp_state, sizeof(HeavyPdWrapper));
    if (!dsp_state->plugindata)
    {
        return FMOD_ERR_MEMORY;
    }
    HeavyPdWrapper *pluginData = (HeavyPdWrapper *)dsp_state->plugindata;
    FMOD_DSP_GETSAMPLERATE(dsp_state, &pluginData->sampleRate);
    pluginData->sampleRateConversionFactor = pluginData->sampleRate / 1000;
    
    /*
    void *rawData;
    FMOD_DSP_GETUSERDATA(dsp_state, &rawData);
    auto *userData = static_cast<UserDataStruct*>(rawData);
    if(userData)
    {
        pluginData->tailLength = userData->tailLength;
        pluginData->isInstrument = userData->isInstrument;
    }
    */
    
    auto userData = static_cast<UserDataStruct*>(FMOD_HEAVYPD_Desc.userdata);
    pluginData->tailLength = userData->tailLength;
    pluginData->isInstrument = userData->isInstrument;
    pluginData->multiChannelExpandable = userData->multiChannelExtendable;
    pluginData->Init();

    
    //FMOD_DSP_LOG(dsp_state, FMOD_DEBUG_LEVEL_LOG, "Create","NumParams is %i", FMOD_HEAVYPD_Desc.numparameters);
    
    /*
    FMOD_DSP_LOG(dsp_state, FMOD_DEBUG_LEVEL_LOG, "Create","Samprate is %i", pluginData->sampleRate);
    FMOD_DSP_LOG(dsp_state, FMOD_DEBUG_LEVEL_LOG, "Create","Tail is %ims", pluginData->tailLength);
    FMOD_DSP_LOG(dsp_state, FMOD_DEBUG_LEVEL_LOG, "Create","Is Instrument: %i", pluginData->isInstrument);
     */
    //FMOD_DSP_LOG(dsp_state, FMOD_DEBUG_LEVEL_LOG, "Create","Tail: %f", pluginData->tailLength);
    //FMOD_DSP_LOG(dsp_state, FMOD_DEBUG_LEVEL_LOG, "Create","Expandable?: %d", pluginData->multiChannelExpandable);
    
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dsprelease(FMOD_DSP_STATE *dsp_state)
{
    HeavyPdWrapper *pluginData = (HeavyPdWrapper *)dsp_state->plugindata;
    /*
    void* userDataRaw;
    FMOD_DSP_GETUSERDATA(dsp_state, &userDataRaw);
    if(userDataRaw)
    {
        delete static_cast<UserDataStruct*>(userDataRaw);
        userDataRaw = nullptr;
    }
    */
    FMOD_DSP_LOG(dsp_state, FMOD_DEBUG_LEVEL_LOG, "Release","%x", pluginData);
    FMOD_DSP_FREE(dsp_state, pluginData);
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dspprocess(FMOD_DSP_STATE *dsp_state, unsigned int length, const FMOD_DSP_BUFFER_ARRAY *inbufferarray, FMOD_DSP_BUFFER_ARRAY *outbufferarray, FMOD_BOOL inputsidle, FMOD_DSP_PROCESS_OPERATION op)
{
    HeavyPdWrapper *pluginData = (HeavyPdWrapper *)dsp_state->plugindata;
    
    auto numChans = pluginData->context[0]->getNumOutputChannels();
    auto numInChans = pluginData->context[0]->getNumInputChannels();
    
    
    if(op == FMOD_DSP_PROCESS_QUERY)
    {
        pluginData->inputsIdle = inputsidle;
        
        if (!outbufferarray)
            return FMOD_ERR_DSP_DONTPROCESS;
        
        if(numInChans > 1)
            if (inbufferarray->buffernumchannels[0] != (int)numInChans)
                return FMOD_ERR_DSP_DONTPROCESS;
        
        if(pluginData->multiChannelExpandable)
        {
            auto chans = inbufferarray->buffernumchannels[0];
            
            if (chans != pluginData->lastChannelCount)
            {
                pluginData->lastChannelCount = chans;
                
                if(pluginData->deInterleaveBuffer)
                    delete[] pluginData->deInterleaveBuffer;
                if(pluginData->interleaveBuffer)
                    delete[] pluginData->interleaveBuffer;
                
                pluginData->deInterleaveBuffer = new float[length * chans];
                pluginData->interleaveBuffer = new float[length * chans];
            }
            
            outbufferarray->speakermode = FMODHelperMethods::GetSpeakermode(chans);
            outbufferarray->buffernumchannels[0] = chans;
        }
        else
        {
            outbufferarray->speakermode = FMODHelperMethods::GetSpeakermode(numChans);
            outbufferarray->buffernumchannels[0] = (int)numChans;
        }
        
        if(inputsidle != pluginData->lastIdleState)
        {
            pluginData->timeStore = pluginData->context[0]->getCurrentSample();
            pluginData->shouldGoIdle = false;
        }
        
        if(inputsidle && pluginData->shouldGoIdle)
            return FMOD_ERR_DSP_DONTPROCESS;
        
        pluginData->lastIdleState = inputsidle;
        
        return FMOD_OK;
    }

    if(pluginData->multiChannelExpandable)
    {
        auto chans = inbufferarray->buffernumchannels[0];
        
        //de-interleave
        for(size_t i(0);i<length;i++)
        {
            for(size_t c(0);c<chans;c++)
            {
                pluginData->deInterleaveBuffer[c*length + i] = inbufferarray[0].buffers[0][chans*i + c];
            }
        }
        
        //process
        for(size_t i(0);i<chans;i++)
        {
            pluginData->context[i]->processInline(&pluginData->deInterleaveBuffer[i*length], &pluginData->interleaveBuffer[i*length], length);
        }
        
        //interleave
        for(size_t c(0);c<chans;c++)
        {
            for(size_t i(0);i<length;i++)
            {
                outbufferarray[0].buffers[0][c + chans*i] = pluginData->interleaveBuffer[c*length + i];
            }
        }
        
    }
    else
    {
        pluginData->context[0]->processInlineInterleaved(inbufferarray[0].buffers[0], outbufferarray[0].buffers[0], length);
    }
    
    
    if(!pluginData->isInstrument && pluginData->inputsIdle)
    {
        if (pluginData->tailLength <= 0)
        {
            pluginData->shouldGoIdle = true;
            return FMOD_OK;
        }
            
        if (CheckIfOutputQuiet(outbufferarray[0].buffers[0], length, numChans))
        {
            pluginData->shouldGoIdle = (pluginData->context[0]->getCurrentSample() - pluginData->timeStore) > (pluginData->tailLength * pluginData->sampleRateConversionFactor);
        }
        else
        {
            pluginData->timeStore = pluginData->context[0]->getCurrentSample();
        }
    }
    /*
    static size_t counter = 0;
    if (counter % 500 == 0)
    {
        FMOD_DSP_LOG(dsp_state, FMOD_DEBUG_LEVEL_LOG, "Proc","Idle: %i", inputsidle);
        FMOD_DSP_LOG(dsp_state, FMOD_DEBUG_LEVEL_LOG, "Proc","Idle: %d", pluginData->context->getCurrentSample());
    }
    counter++;
    */
    return FMOD_OK;
}


FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dspreset(FMOD_DSP_STATE *dsp_state)
{
    
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dspsetparamfloat(FMOD_DSP_STATE *dsp_state, int index, float value)
{
    HeavyPdWrapper *pluginData = (HeavyPdWrapper *)dsp_state->plugindata;
    
    if(index < FMOD_HEAVYPD_Desc.numparameters)
    {
        int indexHash = static_cast<UserDataStruct*>(FMOD_HEAVYPD_Desc.userdata)->parameterIndexToHash[index];
        
        if(pluginData->multiChannelExpandable)
            for(size_t i(0);i<(pluginData->lastChannelCount == 0 ? MAX_CHANS : pluginData->lastChannelCount);i++)
                pluginData->context[i]->sendFloatToReceiver(indexHash, value);
        else
            pluginData->context[0]->sendFloatToReceiver(indexHash, value);
        
        return FMOD_OK;
    }
    
    return FMOD_ERR_INVALID_PARAM;
}

FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dspgetparamfloat(FMOD_DSP_STATE *dsp_state, int index, float *value, char *valuestr)
{
    /*
    HeavyPdWrapper *pluginData = (HeavyPdWrapper *)dsp_state->plugindata;

    if(index < FMOD_HEAVYPD_Desc.numparameters)
    {
        HvParameterInfo info;
        pluginData->context->getParameterInfo(index, &info);
        
    }
    */
    return FMOD_ERR_INVALID_PARAM;
}

FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dspsetparamdata(FMOD_DSP_STATE *dsp_state, int index, void *data, unsigned int /*length*/)
{
    HeavyPdWrapper *pluginData = (HeavyPdWrapper *)dsp_state->plugindata;
    
    void* userDataPtr;
    FMOD_DSP_GETUSERDATA(dsp_state, &userDataPtr);
    UserDataStruct *userData = static_cast<UserDataStruct*>(userDataPtr);
    
    if(userData && index == userData->attributesIndex)
    {
        if(pluginData->multiChannelExpandable)
            for(size_t i(0);i<pluginData->lastChannelCount;i++)
                FMODHelperMethods::Dispatch3DAttributes(pluginData->context[i].get(), data, userData->attributeBitmap);
        else
            FMODHelperMethods::Dispatch3DAttributes(pluginData->context[0].get(), data, userData->attributeBitmap);
        
        return FMOD_OK;
    }
    
    return FMOD_ERR_INVALID_PARAM;
}

FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_dspgetparamdata(FMOD_DSP_STATE *dsp_state, int index, void ** /*value*/, unsigned int * /*length*/, char * /*valuestr*/)
{
    

    return FMOD_ERR_INVALID_PARAM;
}

FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_sys_register(FMOD_DSP_STATE * /*state*/)
{
    FMOD_HEAVYPD_Running = true;
    
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_sys_deregister(FMOD_DSP_STATE * /*state*/)
{
    FMOD_HEAVYPD_Running = false;
    
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK FMOD_HEAVYPD_sys_mix(FMOD_DSP_STATE * /*state*/, int /*stage*/)
{
    return FMOD_OK;
}



#endif /* HeavyPD_FMOD_hpp */
