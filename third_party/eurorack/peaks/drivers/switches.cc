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
// Driver for the split and function switches.

#include "peaks/drivers/switches.h"

#include <string.h>

namespace peaks {

void Switches::Init() {
  memset(&switch_state_, 0xff, sizeof(switch_state_));
  
  GPIO_InitTypeDef gpio_init;

  gpio_init.GPIO_Pin = GPIO_Pin_4;
  gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
  gpio_init.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &gpio_init);
  
  gpio_init.GPIO_Pin = GPIO_Pin_8;
  gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
  gpio_init.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &gpio_init);
  
  gpio_init.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
  gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
  gpio_init.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &gpio_init);
  
}

void Switches::Debounce() {
  switch_state_[3] = (switch_state_[3] << 1) | \
      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
  switch_state_[2] = (switch_state_[2] << 1) | \
      GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8);
  switch_state_[0] = (switch_state_[0] << 1) | \
      GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
  switch_state_[1] = (switch_state_[1] << 1) | \
      GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14);
}

}  // namespace peaks
