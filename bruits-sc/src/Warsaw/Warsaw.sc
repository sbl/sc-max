Warsaw : UGen {
  *ar { |freq = 440, pw = 0.5, shape = 0|
    ^this.multiNew('audio', freq, pw, shape);
  }
  checkInputs {
    ^this.checkValidInputs;
  }
}
