#include "SC_PlugIn.h"

#include "SoftCutHeadLogic.h"

struct SoftCutHead : public Unit {
    float prevTrig;
    float m_fbufnum;
    float m_failedBufNum;
    SndBuf *m_buf;
    SoftCutHeadLogic cutfade; // NB: constructor is never called on this field!
};

static InterfaceTable *ft;

static inline bool checkBuffer(Unit *unit, const float *bufData, uint32 bufChannels,
                               uint32 expectedChannels, int inNumSamples) {
    if (!bufData)
        goto handle_failure;

    if (expectedChannels > bufChannels) {
        if (unit->mWorld->mVerbosity > -1 && !unit->mDone)
            Print("Buffer UGen channel mismatch: expected %i, yet buffer has %i channels\n",
                  expectedChannels, bufChannels);
        goto handle_failure;
    }
    return true;
    handle_failure:
    Print("checkBuffer failed!");
    unit->mDone = true;
    ClearUnitOutputs(unit, inNumSamples);
    return false;
}

static void SoftCutHead_next(SoftCutHead *unit, int inNumSamples);

static void SoftCutHead_Ctor(SoftCutHead *unit);

void SoftCutHead_Ctor(SoftCutHead *unit) {
    unit->cutfade.init();
    unit->cutfade.setSampleRate(SAMPLERATE);
    unit->m_fbufnum = -1e9f;
    unit->m_failedBufNum = -1e9f;
    unit->prevTrig = 0.f;
    SETCALC(SoftCutHead_next);
    SoftCutHead_next(unit, 1);
}

void SoftCutHead_next(SoftCutHead *unit, int inNumSamples) {
    GET_BUF;
    // FIXME: for stereo...
    //    uint32 numOutputs = unit->mNumOutputs;
    uint32 numInputChannels = unit->mNumInputs - 14;

    if (!checkBuffer(unit, bufData, bufChannels, numInputChannels, inNumSamples))
        return;

    unit->cutfade.setBuffer(bufData, bufFrames);

    // AR outputs
    float *phase_out = OUT(0);
    float *trig_out = OUT(1);
    float *snd_out = OUT(2);

    // AR inputs
    const float *in = IN(1);
    const float *rec = IN(2);
    const float *pre = IN(3);
    const float *rate = IN(4);

    // KR inputs
    const float pos = IN0(5);
    const float trig = IN0(6);
    const float start = IN0(7);
    const float end = IN0(8);
    const float fade = IN0(9);
    const float loop = IN0(10);

    const float fadeRec = IN0(11);
    const float fadePre = IN0(12);
    const float recRun = IN0(13);
    const float recOffset= IN0(14);

    unit->cutfade.setLoopStartSeconds(start);
    unit->cutfade.setLoopEndSeconds(end);
    unit->cutfade.setFadeTime(fade);
    unit->cutfade.setLoopFlag(loop > 0);

    unit->cutfade.setFadeRec(fadeRec);
    unit->cutfade.setFadePre(fadePre);
    unit->cutfade.setRecRun(recRun > 0);
    unit->cutfade.setRecOffset(recOffset);


    if ((trig > 0) && (unit->prevTrig <= 0)) {
      // FIXME: i think it will be ok for now,
      // but should convert and wrap this result in the logic class rather than in here.
	    unit->cutfade.cutToPhase(pos * SAMPLERATE);
    }

    unit->prevTrig = trig;

    float snd, phi, tr;
    // trigger should be high/low for the entire block...
    // unfortunately it doesn't seem possible to avoid using audio bus for tr output
    float trBlock = 0.f;

    for (int i = 0; i < inNumSamples; ++i) {
      unit->cutfade.setRec(rec[i]);
      unit->cutfade.setPre(pre[i]);
      unit->cutfade.setRate(rate[i]);

      unit->cutfade.nextSample(in[i], &phi, &tr, &snd);
      phase_out[i] = phi;
      snd_out[i] = snd;
      if(tr > 0.f) { trBlock = 1.f;}
    }

    for (int i = 0; i < inNumSamples; ++i) {
      trig_out[i] = trBlock;
    }
}

PluginLoad(SoftCutHead) {
  ft = inTable;
  DefineSimpleUnit(SoftCutHead);
  Print("PluginLoad(SoftCutHead)\n");
}
