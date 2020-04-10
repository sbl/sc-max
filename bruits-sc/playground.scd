s.boot;
s.reboot;

// softcut


b = Buffer.read(s, Platform.resourceDir +/+ "sounds/a11wlk01.wav");


(
{
  Softcut.ar(b, SoundIn.ar(0));

}.play;
)




// Trigon

(
Ndef(\trigon, {
  Trigon.ar(220, 0.5, 0.5, 0.5)
}).play;
)



// just friends ???


(
Ndef(\jf, {
  FriendZone.ar(100);
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
