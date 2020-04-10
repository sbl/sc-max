Softcut : UGen {
  *ar { |input|
    ^this.multiNew('audio', input);
  }
  checkInputs {
    ^this.checkValidInputs;
  }
}
