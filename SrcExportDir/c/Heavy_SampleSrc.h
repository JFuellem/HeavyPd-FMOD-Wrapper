/** "SomeCopyright" */

#ifndef _HEAVY_SAMPLESRC_H_
#define _HEAVY_SAMPLESRC_H_

#include "HvHeavy.h"

#ifdef __cplusplus
extern "C" {
#endif

#if HV_APPLE
#pragma mark - Heavy Context
#endif

typedef enum {
  HV_SAMPLESRC_PARAM_IN_SYS_TAIL = 0x14D21442, // Sys_tail
  HV_SAMPLESRC_PARAM_IN_VOL = 0x8559698F, // vol
} Hv_SampleSrc_ParameterIn;

typedef enum {
  HV_SAMPLESRC_PARAM_OUT_SENDTESTVOLUME = 0x311E9A92, // sendTestVolume
} Hv_SampleSrc_ParameterOut;


/**
 * Creates a new patch instance.
 * Sample rate should be positive and in Hertz, e.g. 44100.0.
 */
HeavyContextInterface *hv_SampleSrc_new(double sampleRate);

/**
 * Creates a new patch instance.
 * @param sampleRate  Sample rate should be positive (> 0) and in Hertz, e.g. 48000.0.
 * @param poolKb  Pool size is in kilobytes, and determines the maximum amount of memory
 *   allocated to messages at any time. By default this is 10 KB.
 * @param inQueueKb  The size of the input message queue in kilobytes. It determines the
 *   amount of memory dedicated to holding scheduled messages between calls to
 *   process(). Default is 2 KB.
 * @param outQueueKb  The size of the output message queue in kilobytes. It determines the
 *   amount of memory dedicated to holding scheduled messages to the default sendHook.
 *   See getNextSentMessage() for info on accessing these messages. Default is 0 KB.
 */
HeavyContextInterface *hv_SampleSrc_new_with_options(double sampleRate, int poolKb, int inQueueKb, int outQueueKb);

/**
 * Free the patch instance.
 */
void hv_SampleSrc_free(HeavyContextInterface *instance);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // _HEAVY_SAMPLESRC_H_
