Softcut : UGen {
  *ar { arg
    bufnum,
    input = 0,
    rate = 1,
    trigger = 1.0,
    position = 0,
    play = 1,
    loopStart = 0,
    loopEnd = 1,
    loop = 0,
    preLevel = 1,
    recLevel = 1,
    recOffset = 0,
    rec = 0,
    fadeTime = 0.1,
    recPreSlewTime = 0.001,
    rateSlewTime = 0.001
    ;

    ^this.multiNew('audio',
      bufnum,
      input,
      rate,
      trigger,
      position,
      play,
      loopStart,
      loopEnd,
      loop,
      preLevel,
      recLevel,
      recOffset,
      rec,
      fadeTime,
      recPreSlewTime,
      rateSlewTime,
    );
  }
  checkInputs {
    ^this.checkValidInputs;
  }
}
