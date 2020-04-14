Softcut : UGen {
  // void setRate(float rate);
  // void cutToPos(float sec);
  // void setLoopStart(float sec);
  // void setLoopEnd(float sec);
  // void setLoopFlag(bool val);
  // void setPreLevel(float amp);
  // void setRecLevel(float amp);
  // void setRecOffset(float d);
  // void setRecFlag(bool val);
  // void setPlayFlag(bool val);

  // void setFadeTime(float sec);
  // void setRecPreSlewTime(float d);
  // void setRateSlewTime(float d);

  *ar { arg
    bufnum,
    input = 0,
    rate = 1,
    position = 0,
    play = 1,
    loopStart = 0,
    loopEnd = 1,
    loop = 1,
    preLevel = 1,
    recLevel = 1,
    recOffset = 0,
    rec = 0,
    fadeTime = 0.1,
    recPreSlewTime = 0.001,
    rateSlewTime = 0.001,
    ;

    ^this.multiNew('audio',
      bufnum,
      input,
      rate,
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
