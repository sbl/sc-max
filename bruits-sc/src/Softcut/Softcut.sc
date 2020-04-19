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
    rec = 0,
    preFilterFc = 16000,
    preFilterRq = 4,
    preFilterLp = 0,
    preFilterHp = 0,
    preFilterBp = 0,
    preFilterBr = 0,
    preFilterDry = 1,
    postFilterFc = 12000,
    postFilterRq = 4,
    postFilterLp = 0,
    postFilterHp = 0,
    postFilterBp = 0,
    postFilterBr = 0,
    postFilterDry = 1,
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
      rec,
      preFilterFc,
      preFilterRq,
      preFilterLp,
      preFilterHp,
      preFilterBp,
      preFilterBr,
      preFilterDry,
      postFilterFc,
      postFilterRq,
      postFilterLp,
      postFilterHp,
      postFilterBp,
      postFilterBr,
      postFilterDry,
      fadeTime,
      recPreSlewTime,
      rateSlewTime,
    );
  }

  *initClass {
    SynthDef(\softcut, {
      arg
      in = 0,
      out = 0,
      pan = 0,
      level = 1.0,

      buffer,
      rate = 1,
      pos = 0,
      play = 1,
      loopStart = 0,
      loopEnd = 1,
      loop = 0,
      preLevel = 0.9,
      recLevel = 0.9,
      rec = 0,
      preFilterFc = 16000,
      preFilterRq = 4,
      preFilterLp = 0,
      preFilterHp = 0,
      preFilterBp = 0,
      preFilterBr = 0,
      preFilterDry = 1,
      postFilterFc = 12000,
      postFilterRq = 4,
      postFilterLp = 0,
      postFilterHp = 0,
      postFilterBp = 0,
      postFilterBr = 0,
      postFilterDry = 1,

      fadeTime = 0.1,
      recPreSlewTime = 0.01,
      rateSlewTime = 0.01,
      levelSlewTime = 0.01
      ;

      var trigger = \trigger.tr(0);
      var sndInput = SoundIn.ar(in);

      var snd = Softcut.ar(buffer,
        sndInput,
        rate,
        trigger,
        pos,
        play,
        loopStart,
        loopEnd,
        loop,
        preLevel,
        recLevel,
        rec,
        preFilterFc,
        preFilterRq,
        preFilterLp,
        preFilterHp,
        preFilterBp,
        preFilterBr,
        preFilterDry,
        postFilterFc,
        postFilterRq,
        postFilterLp,
        postFilterHp,
        postFilterBp,
        postFilterBr,
        postFilterDry,
        fadeTime,
        recPreSlewTime,
        rateSlewTime
      );

      snd = Pan2.ar(snd, pan, level.lag(levelSlewTime));
      Out.ar(out, snd);
    }).add;
  }

  checkInputs {
    ^this.checkValidInputs;
  }
}
