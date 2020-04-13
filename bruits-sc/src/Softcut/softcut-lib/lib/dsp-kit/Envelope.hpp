//
// Created by emb on 2/3/20.
//
// general-purpose envelope class,
// with arbitrary sequencing behavior and shape functions./
// suitable for audio DSP, but NOT threadsafe!

#ifndef DSP_KIT_ENVELOPE_HPP
#define DSP_KIT_ENVELOPE_HPP

#include <functional>

#include "easing.hpp"
#include "Fifo.hpp"

class Envelope {

public:
    struct Stage {
        float target;
        float time;
        easing::function shape;
    };

    enum class MoveState {
        idle,
        moving
    };

    enum class SequenceMode {
        // consume each stage (FIFO), then stop
                queue,
        // loop over stages forever
                loop,
        // play all stages, then stop
            oneshot,
    };


    void setValue(float val);

public:
    Envelope();

    // change the sample rate
    // (NB: this will only take effect on next stage or `execute`)
    void setSampleRate(float sr);

    // set the loop mode
    void setSequenceMode(SequenceMode mode);

    // return the next sample value
    float processSample();

    // set the default shape for rising segments
    float setRiseShape(easing::function shape = easing::function::linear);

    // set the default shape for falling segments
    float setFallShape(easing::function shape = easing::function::linear);

    // set callback function
    float setCallback(std::function<void(float)> fn);

    // immediately execute a movement with given stage data.
    // if a stage is currently being executed, this will pivot from the current value
    void go(float target, float time, easing::function shape = easing::function::none);

    // queue up a stage for processing
    void addStage(float target, float time, easing::function shape = easing::function::none);

    // remove the next stage without executing it. (doesn't cancel current stage, if any)
    void popStage();

    // remove all queued stages. (doesn't cancel current stage, if any)
    void clearStages();

    // immediately advance to the next stage (possibly looping)
    void nextStage();

private:
    void updateValue();

    void updatePhase();

private:
    static constexpr int maxStages = 32;
    dspkit::Fifo<Stage, maxStages> stageQ;
    MoveState moveState; // current state (idle/moving)
    SequenceMode sequenceMode; // current sequence mode

    easing::function riseShape; // default shape for rising segments
    easing::function fallShape; // default shape for falling segments
    std::function<float(float)> shapeFunc; // current easing function
    std::function<void(float)> callback; // current callback function

    float phase; // current phase in [0,1]
    float value; // current output value
    float start;   // start point, for scaling
    float end;   // end point, for scaling
    float scale; // just (end-start)
    float inc; // phase increment per sample
    float sr; // samplerate
};


#endif //DSP_KIT_ENVELOPE_HPP
