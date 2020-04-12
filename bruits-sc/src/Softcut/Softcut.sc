Softcut : UGen {
  *ar { |bufnum, input = 0, position = 0|
    ^this.multiNew('audio', bufnum, input, position);
  }
  checkInputs {
    ^this.checkValidInputs;
  }
}
