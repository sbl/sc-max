WRShaper : UGen {
  *ar { |input, shape = 0.5|
    ^this.multiNew('audio', input, shape);
  }
  checkInputs {
    ^this.checkValidInputs;
  }
}
