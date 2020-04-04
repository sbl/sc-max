// Copyright 2013 Emilie Gillet.
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
// 808-style HH.
 
#ifndef PEAKS_DRUMS_HIGH_HAT_H_
#define PEAKS_DRUMS_HIGH_HAT_H_

#include "stmlib/stmlib.h"

#include "peaks/drums/svf.h"
#include "peaks/drums/excitation.h"

#include "peaks/gate_processor.h"

namespace peaks {

class HighHat {
 public:
  HighHat() { }
  ~HighHat() { }

  void Init();
  void Process(const GateFlags* gate_flags, int16_t* out, size_t size);
  void Configure(uint16_t* parameter, ControlMode control_mode) { }
  
 private:
  Svf noise_;
  Svf vca_coloration_;
  Excitation vca_envelope_;
  
  uint32_t phase_[6];

  DISALLOW_COPY_AND_ASSIGN(HighHat);
};

}  // namespace peaks

#endif  // PEAKS_DRUMS_HIGH_HAT_H_
