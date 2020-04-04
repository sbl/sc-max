Griesinger : MultiOutUGen {
  *ar { |input, amount = 0.5, time = 0.5, lp = 0.5, diffusion = 0.7 |
    ^this.multiNew('audio', input, amount, time, lp, diffusion);
  }

  init { |... theInputs|
      inputs = theInputs;
      ^this.initOutputs(2, rate);
  }

  checkInputs {
    ^this.checkValidInputs;
  }

}
