FriendZone : UGen {
  *ar { |rate = 50, sustain = 1|
    ^this.multiNew('audio', rate, sustain);
  }
  checkInputs {
    ^this.checkValidInputs;
  }
}
