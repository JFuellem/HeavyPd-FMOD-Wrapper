/** "SomeCopyright" */

#ifndef _HEAVY_CONTEXT_SAMPLESRC_HPP_
#define _HEAVY_CONTEXT_SAMPLESRC_HPP_

// object includes
#include "HeavyContext.hpp"
#include "HvSignalSample.h"
#include "HvSignalVar.h"
#include "HvSignalPhasor.h"
#include "HvSignalLine.h"
#include "HvMath.h"

class Heavy_SampleSrc : public HeavyContext {

 public:
  Heavy_SampleSrc(double sampleRate, int poolKb=10, int inQueueKb=2, int outQueueKb=0);
  ~Heavy_SampleSrc();

  const char *getName() override { return "SampleSrc"; }
  int getNumInputChannels() override { return 1; }
  int getNumOutputChannels() override { return 1; }

  int process(float **inputBuffers, float **outputBuffer, int n) override;
  int processInline(float *inputBuffers, float *outputBuffer, int n) override;
  int processInlineInterleaved(float *inputBuffers, float *outputBuffer, int n) override;

  int getParameterInfo(int index, HvParameterInfo *info) override;
  struct Parameter {
    struct In {
      enum ParameterIn : hv_uint32_t {
        SYS_TAIL = 0x14D21442, // Sys_tail
        VOL = 0x8559698F, // vol
      };
    };
    struct Out {
      enum ParameterOut : hv_uint32_t {
        SENDTESTVOLUME = 0x311E9A92, // sendTestVolume
      };
    };
  };

 private:
  HvTable *getTableForHash(hv_uint32_t tableHash) override;
  void scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) override;

  // static sendMessage functions
  static void sSample_TsQLJbpI_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_dsRM3B4K_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_BdWW7IeP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSend_ZpamRBG9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_g3etf9Uf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_LdjqwoQZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);

  // objects
  SignalLine sLine_ZMQbuccH;
  SignalPhasor sPhasor_MNXiNfyY;
  SignalSample sSample_TsQLJbpI;
};

#endif // _HEAVY_CONTEXT_SAMPLESRC_HPP_
