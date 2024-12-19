// !!!! select usb type: serial + audio + mid
#include <Audio.h>
#include <MIDI.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
//https://github.com/waspinator/CD74HC4067
#include "Parameter.h"
// #include "config.h"
// #include "voice.h"

#include "AudioSampleKick.h"

AudioControlSGTL5000 _audioShield;
AudioInputI2S _lineInput;
AudioInputUSB _usbInput;
AudioPlayMemory _playMem;

AudioOutputUSB _usbOutput;
AudioOutputI2S _lineOutput;

AudioMixer4 _leftMixer;
AudioMixer4 _rightMixer;

AudioAmplifier _leftAmp;
AudioAmplifier _rightAmp;

AudioOutputI2S _headphones;
AudioOutputAnalog _dac;


AudioConnection _lineToMixerLeft(_lineInput, 0, _leftMixer, 0);
AudioConnection _lineToMixerRight(_lineInput, 1, _rightMixer, 0);

AudioConnection _usbToMixerLeft(_usbInput, 0, _leftMixer, 1);
AudioConnection _usbToMixerRight(_usbInput, 1, _rightMixer, 1);

AudioConnection _playMemToLeftMixer(_playMem, 0, _leftMixer, 2);
AudioConnection _playMemToRightMixer(_playMem, 0, _rightMixer, 2);

AudioConnection _leftMixerToUsbL(_leftMixer, 0, _usbOutput, 0);
AudioConnection _rightMixerToUsbR(_rightMixer, 0, _usbOutput, 1);

AudioConnection _mixerToHeadphonesLeft(_leftMixer, 0, _headphones, 0);
AudioConnection _mixerToHeadphonesRight(_rightMixer, 0, _headphones, 1);
AudioConnection _mixerToDacLeft(_leftMixer, 0, _dac, 0);
AudioConnection _mixerToDacRight(_rightMixer, 0, _dac, 1);



void audioTest() {
  delay(250);
}


void onNoteOn(byte channel, byte note, byte velocity) {
  _playMem.play(AudioSampleKick);
  Serial.println("got note on");
}
void onNoteOff(byte channel, byte note, byte velocity) {
}
void onControlChange(byte channel, byte controller, byte value) {
  switch (controller) {
    case 1:
      {
        break;
      }
    case 2:
      {
        break;
      }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.setClock(400000);  // Set I2C to 400 kHz

  // midi
  Serial.println("setup midi engine");
  usbMIDI.setHandleNoteOff(onNoteOff);
  usbMIDI.setHandleNoteOn(onNoteOn);
  usbMIDI.setHandleControlChange(onControlChange);

  // audio
  Serial.println("setup audio engine");
  AudioMemory(128);
  _audioShield.enable();
  _audioShield.volume(1);
  _audioShield.inputSelect(AUDIO_INPUT_LINEIN);
  _audioShield.lineInLevel(15);  // Adjust the level as needed

  for (auto i = 0; i < 4; i++) {
    _leftMixer.gain(i, 0.4);
    _rightMixer.gain(i, 0.4);
  }
}

void loop() {
  auto timestamp = millis();
  usbMIDI.read();
  Serial.println("test");
  usbMIDI.sendNoteOn(60, 127, 1);
  delay(1000);
}
