// Mutable Instruments Tides 2
Trigon : MultiOutUGen {
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
    smoothness = 0.5;

    ^this.multiNew('audio',
        freq.cpsmidi,
        shape,
        slope,
        smoothness,
    );
  }


  init { |... theInputs|
      inputs = theInputs;
      ^this.initOutputs(1, rate);
  }

  checkInputs {
    ^this.checkValidInputs;
  }
}
