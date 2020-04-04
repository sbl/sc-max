MS20 : UGen {
  *ar { |input, freq = 440, peak = 0, drive = 0|
    ^this.multiNew('audio', input, freq, peak, drive);
  }
  checkInputs {
    ^this.checkValidInputs;
  }
}
