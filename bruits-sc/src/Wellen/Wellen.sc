// Mutable Instruments Tides 2
Wellen : MultiOutUGen {
  const <modeAD = 0;
  const <modeLooping = 1;
  const <modeAR = 2;

  const <outputGates = 0;
  const <outputAmplitude = 1;
  const <outputSlope = 2;
  const <outputFrequency = 3;

  *ar { arg
    freq = 440,
    shape = 0.5,
    slope = 0.5,
    smoothness = 0.5,
    shift = 1,
    trigger = 0,
    // ar, looping, as
    rampMode = Wellen.modeLooping,
    outputMode = Wellen.outputGates
    ;

    ^this.multiNew('audio',
        freq.cpsmidi,
        shape,
        slope,
        smoothness,
        shift,
        trigger,
        rampMode,
        outputMode
    );
  }


  init { |... theInputs|
      inputs = theInputs;
      ^this.initOutputs(4, rate);
  }

  checkInputs {
    ^this.checkValidInputs;
  }
}
