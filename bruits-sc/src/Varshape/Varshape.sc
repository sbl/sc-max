Varshape : UGen {
  *ar { |freq = 0.01, pw = 0.5, shape = 0|
    ^this.multiNew('audio', freq, pw, shape);
  }
  checkInputs {
    ^this.checkValidInputs;
  }
}
