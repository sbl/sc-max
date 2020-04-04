// Copyright 2017 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// Recovers a ramp from a clock input by guessing at what time the next edge
// will occur. Prediction strategies:
// - Moving average of previous intervals.
// - Periodic rhythmic pattern.
// - Assume that the pulse width is constant, deduct the period from the on time
//   and the pulse width.
// 
// All prediction strategies are concurrently tested, and the output from the
// best performing one is selected (à la early Scheirer/Goto beat trackers).

#ifndef STAGES_RAMP_EXTRACTOR_H_
#define STAGES_RAMP_EXTRACTOR_H_

#include "stmlib/stmlib.h"

#include "stmlib/utils/gate_flags.h"

namespace stages {

struct Ratio {
  float ratio;  // Precomputed p/q
  int q;
};

const int kMaxPatternPeriod = 8;

class RampExtractor {
 public:
  RampExtractor() { }
  ~RampExtractor() { }
  
  void Init(float sample_rate, float max_frequency);
  void Process(
      Ratio r,
      const stmlib::GateFlags* gate_flags,
      float* ramp,
      size_t size);
  
 private:
  struct Pulse {
    uint32_t on_duration;
    uint32_t total_duration;
    float pulse_width;
  };
  
  static const size_t kHistorySize = 16;

  float ComputeAveragePulseWidth(float tolerance) const;
  
  float PredictNextPeriod();

  size_t current_pulse_;
  Pulse history_[kHistorySize];
  
  float lp_period_;
  float prediction_error_[kMaxPatternPeriod + 1];
  float predicted_period_[kMaxPatternPeriod + 1];
  float average_pulse_width_;
  
  float train_phase_;
  float frequency_;
  
  uint32_t reset_interval_;
  int reset_counter_;
  float max_ramp_value_;
  float f_ratio_;
  float max_train_phase_;
  
  float max_frequency_;
  float min_period_;
  float min_period_hysteresis_;
  
  DISALLOW_COPY_AND_ASSIGN(RampExtractor);
};

}  // namespace stages

#endif  // STAGES_RAMP_EXTRACTOR_H_
