Zopf : MultiOutUGen {
  *ar { arg
      pitch = 0,
      harmonics = 0,
      timbre = 0.5,
      morph = 0.5,
      engine = 0,
      trigger = 0,
      lpgColor = 0.5,
      lpgDecay = 0.5,
      fmAmount = 0
      ;
    ^this.multiNew('audio', pitch, harmonics, timbre, morph, engine,
        trigger, lpgColor, lpgDecay, fmAmount
    );
  }

  init { |... theInputs|
      inputs = theInputs;
      ^this.initOutputs(2, rate);
  }

  checkInputs {
    ^this.checkValidInputs;
  }
}
