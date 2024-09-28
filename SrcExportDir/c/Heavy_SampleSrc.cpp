/**
 * Copyright (c) 2024 Enzien Audio, Ltd.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the phrase "powered by heavy",
 *    the heavy logo, and a hyperlink to https://enzienaudio.com, all in a visible
 *    form.
 * 
 *   2.1 If the Application is distributed in a store system (for example,
 *       the Apple "App Store" or "Google Play"), the phrase "powered by heavy"
 *       shall be included in the app description or the copyright text as well as
 *       the in the app itself. The heavy logo will shall be visible in the app
 *       itself as well.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include "Heavy_SampleSrc.hpp"

#include <new>

#define Context(_c) static_cast<Heavy_SampleSrc *>(_c)


/*
 * C Functions
 */

extern "C" {
  HV_EXPORT HeavyContextInterface *hv_SampleSrc_new(double sampleRate) {
    // allocate aligned memory
    void *ptr = hv_malloc(sizeof(Heavy_SampleSrc));
    // ensure non-null
    if (!ptr) return nullptr;
    // call constructor
    new(ptr) Heavy_SampleSrc(sampleRate);
    return Context(ptr);
  }

  HV_EXPORT HeavyContextInterface *hv_SampleSrc_new_with_options(double sampleRate,
      int poolKb, int inQueueKb, int outQueueKb) {
    // allocate aligned memory
    void *ptr = hv_malloc(sizeof(Heavy_SampleSrc));
    // ensure non-null
    if (!ptr) return nullptr;
    // call constructor
    new(ptr) Heavy_SampleSrc(sampleRate, poolKb, inQueueKb, outQueueKb);
    return Context(ptr);
  }

  HV_EXPORT void hv_SampleSrc_free(HeavyContextInterface *instance) {
    // call destructor
    Context(instance)->~Heavy_SampleSrc();
    // free memory
    hv_free(instance);
  }
} // extern "C"







/*
 * Class Functions
 */

Heavy_SampleSrc::Heavy_SampleSrc(double sampleRate, int poolKb, int inQueueKb, int outQueueKb)
    : HeavyContext(sampleRate, poolKb, inQueueKb, outQueueKb) {
  numBytes += sLine_init(&sLine_saKGjAn1);
  
}

Heavy_SampleSrc::~Heavy_SampleSrc() {
  // nothing to free
}

HvTable *Heavy_SampleSrc::getTableForHash(hv_uint32_t tableHash) {
  return nullptr;
}

void Heavy_SampleSrc::scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) {
  switch (receiverHash) {
    case 0x14D21442: { // Sys_tail
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_b2c2VWmf_sendMessage);
      break;
    }
    case 0xB22FD2ED: { // Syst_tail_test
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_cMf69FNK_sendMessage);
      break;
    }
    case 0x8559698F: { // vol
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_FayiFbnB_sendMessage);
      break;
    }
    default: return;
  }
}

int Heavy_SampleSrc::getParameterInfo(int index, HvParameterInfo *info) {
  if (info != nullptr) {
    switch (index) {
      case 0: {
        info->name = "Sys_tail";
        info->hash = 0x14D21442;
        info->type = HvParameterType::HV_PARAM_TYPE_PARAMETER_IN;
        info->minVal = 1000.0f;
        info->maxVal = 1000.0f;
        info->defaultVal = 1000.0f;
        break;
      }
      case 1: {
        info->name = "vol";
        info->hash = 0x8559698F;
        info->type = HvParameterType::HV_PARAM_TYPE_PARAMETER_IN;
        info->minVal = 0.0f;
        info->maxVal = 1.0f;
        info->defaultVal = 0.5f;
        break;
      }
      default: {
        info->name = "invalid parameter index";
        info->hash = 0;
        info->type = HvParameterType::HV_PARAM_TYPE_PARAMETER_IN;
        info->minVal = 0.0f;
        info->maxVal = 0.0f;
        info->defaultVal = 0.0f;
        break;
      }
    }
  }
  return 2;
}



/*
 * Send Function Implementations
 */


void Heavy_SampleSrc::cMsg_KBrNyPIP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
}

void Heavy_SampleSrc::cMsg_ThVlHGPn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 50.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_saKGjAn1, 0, m, NULL);
}

void Heavy_SampleSrc::cMsg_1I7kmr8J_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
}

void Heavy_SampleSrc::cReceive_cMf69FNK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_1I7kmr8J_sendMessage(_c, 0, m);
}

void Heavy_SampleSrc::cReceive_FayiFbnB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_ThVlHGPn_sendMessage(_c, 0, m);
}

void Heavy_SampleSrc::cReceive_b2c2VWmf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_KBrNyPIP_sendMessage(_c, 0, m);
}




/*
 * Context Process Implementation
 */

int Heavy_SampleSrc::process(float **inputBuffers, float **outputBuffers, int n) {
  while (hLp_hasData(&inQueue)) {
    hv_uint32_t numBytes = 0;
    ReceiverMessagePair *p = reinterpret_cast<ReceiverMessagePair *>(hLp_getReadBuffer(&inQueue, &numBytes));
    hv_assert(numBytes >= sizeof(ReceiverMessagePair));
    scheduleMessageForReceiver(p->receiverHash, &p->msg);
    hLp_consume(&inQueue);
  }

  sendBangToReceiver(0xDD21C0EB); // send to __hv_bang~ on next cycle
  const int n4 = n & ~HV_N_SIMD_MASK; // ensure that the block size is a multiple of HV_N_SIMD

  // temporary signal vars
  hv_bufferf_t Bf0;

  // input and output vars
  hv_bufferf_t O0;
  hv_bufferf_t I0;

  // declare and init the zero buffer
  hv_bufferf_t ZERO; __hv_zero_f(VOf(ZERO));

  hv_uint32_t nextBlock = blockStartTimestamp;
  for (int n = 0; n < n4; n += HV_N_SIMD) {

    // process all of the messages for this block
    nextBlock += HV_N_SIMD;
    while (mq_hasMessageBefore(&mq, nextBlock)) {
      MessageNode *const node = mq_peek(&mq);
      node->sendMessage(this, node->let, node->m);
      mq_pop(&mq);
    }

    // load input buffers
    __hv_load_f(inputBuffers[0]+n, VOf(I0));

    // zero output buffers
    __hv_zero_f(VOf(O0));

    // process all signal functions
    __hv_line_f(&sLine_saKGjAn1, VOf(Bf0));
    __hv_mul_f(VIf(I0), VIf(Bf0), VOf(Bf0));
    __hv_add_f(VIf(Bf0), VIf(O0), VOf(O0));

    // save output vars to output buffer
    __hv_store_f(outputBuffers[0]+n, VIf(O0));
  }

  blockStartTimestamp = nextBlock;

  return n4; // return the number of frames processed

}

int Heavy_SampleSrc::processInline(float *inputBuffers, float *outputBuffers, int n4) {
  hv_assert(!(n4 & HV_N_SIMD_MASK)); // ensure that n4 is a multiple of HV_N_SIMD

  // define the heavy input buffer for 1 channel(s)
  float **const bIn = &inputBuffers;

  // define the heavy output buffer for 1 channel(s)
  float **const bOut = &outputBuffers;

  int n = process(bIn, bOut, n4);
  return n;
}

int Heavy_SampleSrc::processInlineInterleaved(float *inputBuffers, float *outputBuffers, int n4) {
  hv_assert(n4 & ~HV_N_SIMD_MASK); // ensure that n4 is a multiple of HV_N_SIMD

  // define the heavy input buffer for 1 channel(s), uninterleave
  float *const bIn = inputBuffers;

  // define the heavy output buffer for 1 channel(s)
  float *const bOut = outputBuffers;

  int n = processInline(bIn, bOut, n4);

  

  return n;
}
