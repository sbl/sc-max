Softcut : UGen {
  *ar { |bufnum, input = 0, rate = 1|
    ^this.multiNew('audio', bufnum, input, rate);
  }
  checkInputs {
    ^this.checkValidInputs;
  }
}
