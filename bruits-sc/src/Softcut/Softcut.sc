Softcut : UGen {
  *ar { |bufnum, input = 0|
    ^this.multiNew('audio', bufnum, input);
  }
  checkInputs {
    ^this.checkValidInputs;
  }
}
