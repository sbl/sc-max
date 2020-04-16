s.boot;
s.reboot;

// softcut


b = Buffer.read(s, Platform.resourceDir +/+ "sounds/a11wlk01.wav", 0, 2 ** 16, bufnum: 0);
b = Buffer.read(s, "/Users/sbl/Desktop/sport.wav", 0, 2 ** 16, bufnum: 0);
b = Buffer.read(s, "/Users/sbl/Library/Mobile Documents/com~apple~CloudDocs/audio/samples/recs/NotwistPianpo.wav", 0, 2 ** 17, bufnum: 0);


(
s.reboot;
s.waitForBoot {
  fork {
    b = Buffer.read(s, "/Users/sbl/Library/Mobile Documents/com~apple~CloudDocs/audio/samples/recs/NotwistPianpo.wav", 0, 2 ** 17, bufnum: 0);

    s.sync;

    Ndef(\cutter, { arg rate = 1, t_trig = 1, pos = 0, play = 1,
      loopStart = 0, loopEnd = 1, loop = 0,
      preLevel = 1, recLevel = 1, recOffset = 0, rec = 0,
      fadeTime = 0.1, recPreSlewTime = 0.01, rateSlewTime = 0.01;

      Softcut.ar(b, 0, rate, t_trig, pos, play, loopStart, loopEnd, loop,
        preLevel, recLevel, recOffset, rec,
        fadeTime, recPreSlewTime, rateSlewTime);
    }).play;

  };
};
)

Ndef(\cutter).set(\rate, 2.1);

Ndef(\cutter).set(\pos, 0, \t_trig, 1, \loop, 1, \loopStart, 0, \fadeTime, 0.4, \rateSlewTime, 0.2);

Ndef(\cutter).set(\loopStart, 0, \loopEnd, 3);


(
Tdef(\foo, {

  var seq = Pseq([-1,-1.1,1.2,-1.5], inf).asStream;

  loop {
    Ndef(\cutter).set(\rate, seq.next);
    2.wait;
  }


}).play;


)



// Trigon

(
Ndef(\trigon, {
  Trigon.ar(220, 0.5, 0.5, 0.5)
}).play;
)




// ms20

s.reboot;

(
Ndef(\ms20, {|freq = 440, peak = 0.6, drive = 0.2|
  // freq = Saw.ar(SinOsc.kr(0.2).range(4, 30)).exprange(89, 1200);
  MS20.ar(WhiteNoise.ar, freq, peak, drive) * 0.2;
}).play(0,2);
)



// warsaw

(
Ndef(\snd, { |freq = 250|
  Warsaw.ar(freq, LFSaw.ar(1.6).range(0,1));
}).play(0,2);
)



// VCM Show off

(
Ndef(\falter, { |gain = 1|
  var lfo = LFTri.ar(0.2);
  var in = Wellen.ar(50 + LFNoise2.ar(100, 1), 0.2, lfo, 0.2)[0] * 0.5;

  var snd = VCM.ar(in * gain);

  (snd * 0.3).softclip;
}).play(0, 2);
)

Ndef(\falter).set(\gain, 12);


// zopf 1

s.reboot;

(
Ndef(\zopf, { |pitch = -2, harmonics = 0.3, timbre = 0.3, morph = 0.3, engine = 6|

  timbre = SinOsc.kr(0.3).range(0.2, 0.4);
  morph= SinOsc.kr(0.5).range(0.6, 0.7);


  Zopf.ar(pitch, harmonics, timbre, morph, engine)

}).play;
)

Ndef(\zopf).set(\harmonics, 0.3)
Ndef(\zopf).set(\pitch, 0)

// zopf 2

(
Ndef(\zopf2, { |pitch = -2, harmonics = 0.3, timbre = 0.3, morph = 0.3, engine = 3|
  timbre = SinOsc.kr(0.3).range(0.2, 0.4);
  morph= SinOsc.kr(0.5).range(0.6, 0.7);

  Zopf.ar(pitch, harmonics, timbre, morph, engine)
}).play;
)
