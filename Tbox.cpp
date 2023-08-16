#include "Tbox.H"
#include "Arduino.h"


Tbox::Tbox(){
}
void Tbox::begin(){
    Serial.begin(9600);
}

void Tbox::attach_O2(int pin){
    _O2_PIN = pin;
    pinMode(_O2_PIN, OUTPUT);
    digitalWrite(_O2_PIN,LOW);
}
void Tbox::attach_RA(int pin){
    _RA_PIN = pin;
    pinMode(_RA_PIN, OUTPUT);
    digitalWrite(_RA_PIN,LOW);
}
void Tbox::attach_HC(int pin){
    _HC_PIN = pin;
    pinMode(_HC_PIN, OUTPUT);
    digitalWrite(_HC_PIN,LOW);
}
void Tbox::attach_HO(int pin){
    _HO_PIN = pin;
    pinMode(_HO_PIN, OUTPUT);
    digitalWrite(_HO_PIN,LOW);
}
void Tbox::attach_N2(int pin){
    _N2_PIN = pin;
    pinMode(_N2_PIN, OUTPUT);
    digitalWrite(_N2_PIN,LOW);
}
void Tbox::attach_CPAP(int pin){
    _CPAP_PIN = pin;
    pinMode(_CPAP_PIN, OUTPUT);
    digitalWrite(_CPAP_PIN,HIGH);
}
void Tbox::attach_REC(int pin){
    _REC_PIN = pin;
    pinMode(_REC_PIN, OUTPUT);
    digitalWrite(_REC_PIN,LOW);
}

void Tbox::attach_TONE(int pin){
    _TONE_PIN = pin;
    pinMode(_TONE_PIN, OUTPUT);
    digitalWrite(_TONE_PIN,LOW);
}

void Tbox::attachDefaults(){
    attach_O2(_O2_PIN_DEFAULT);
    attach_RA(_RA_PIN_DEFAULT);
    attach_HC(_HC_PIN_DEFAULT);
    attach_HO(_HO_PIN_DEFAULT);
    attach_N2(_N2_PIN_DEFAULT);
    attach_CPAP(_CPAP_PIN_DEFAULT);
    attach_REC(_REC_PIN_DEFAULT);
    attach_TONE(_TONE_PIN_DEFAULT);
    digitalWrite(_O2_PIN,HIGH);
}

void Tbox::open_O2(){
    Serial.println("\tDelivering O2");
    digitalWrite(_O2_PIN,HIGH);
    digitalWrite(_RA_PIN,LOW);
    digitalWrite(_HC_PIN,LOW);
    digitalWrite(_HO_PIN,LOW);
    digitalWrite(_N2_PIN,LOW);
}

void Tbox::open_RA(){
    Serial.println("\tDelivering Room air");
    digitalWrite(_O2_PIN,LOW);
    digitalWrite(_RA_PIN,HIGH);
    digitalWrite(_HC_PIN,LOW);
    digitalWrite(_HO_PIN,LOW);
    digitalWrite(_N2_PIN,LOW);
}

void Tbox::open_HC(){
    Serial.println("\tDelivering Hypercapnia");
    digitalWrite(_O2_PIN,LOW);
    digitalWrite(_RA_PIN,LOW);
    digitalWrite(_HC_PIN,HIGH);
    digitalWrite(_HO_PIN,LOW);
    digitalWrite(_N2_PIN,LOW);
}

void Tbox::open_HO(){
    Serial.println("\tDelivering Hypoxia");
    digitalWrite(_O2_PIN,LOW);
    digitalWrite(_RA_PIN,LOW);
    digitalWrite(_HC_PIN,LOW);
    digitalWrite(_HO_PIN,HIGH);
    digitalWrite(_N2_PIN,LOW);
}

void Tbox::open_N2(){
    Serial.println("\tDelivering Pure Nitrogen");
    digitalWrite(_O2_PIN,LOW);
    digitalWrite(_RA_PIN,LOW);
    digitalWrite(_HC_PIN,LOW);
    digitalWrite(_HO_PIN,LOW);
    digitalWrite(_N2_PIN,HIGH);
}

void Tbox::user_wait(){
    while (Serial.available()) {Serial.read();} // FLush Serial need this to allow for another recording to be done
    delay(1000);
    Serial.println("Enter any key to start protocol.");
    while (!Serial.available()){}
}

void Tbox::probe_settle(float probe_settle_min){
  Serial.print("Waiting for the probe to settle: ");
  Serial.print(probe_settle_min);
  Serial.println("min");
  uint probe_settle_ms = uint(probe_settle_min * 60 * 1000);
  uint t_start_pause = millis();
  elapsedMillis updateTimer=100000;// Make this large so we print right away
  bool alerted=false;
  while ((millis()-t_start_pause)<probe_settle_ms){
    if (updateTimer>=30000){
    _printTimeRemaining(t_start_pause,probe_settle_ms);
    updateTimer=0;
    }
    if ((int(millis())-int(t_start_pause) - int(probe_settle_ms))>-30000){
        if (!alerted){
            playAlert();
            _printTimeRemaining(t_start_pause,probe_settle_ms);
            alerted=true;
            updateTimer=0;
        }
    }
  }
}

void Tbox::start_recording(){
  Serial.println("Starting Recording");
  digitalWrite(_REC_PIN,HIGH);

}

void Tbox::stop_recording(){
    Serial.println("Stopping recording");
    digitalWrite(_REC_PIN,LOW); 
    open_O2();
    Serial.println("======================");
  }

void Tbox::hering_breuer(uint n_reps, uint dur_ms, uint interstim_ms) {
  // Run the Hering Breuer 
  Serial.print("Running H.B. Duration: ");
  Serial.print(dur_ms);
  Serial.println(" ms.");
  for (uint ii = 0; ii < n_reps; ii++) {
    Serial.print("\tHB rep ");
    Serial.print(ii + 1);
    Serial.print(" of ");
    Serial.println(n_reps);
    digitalWrite(_CPAP_PIN, LOW);
    delay(dur_ms);
    digitalWrite(_CPAP_PIN, HIGH);
    delay(interstim_ms);
  }
}

void Tbox::_printTimeRemaining(uint startTime, uint timeTarget) {

    uint elapsedTime = millis()-startTime;
    uint timeRemaining = timeTarget - elapsedTime;
    unsigned long seconds = timeRemaining / 1000;
    unsigned long minutes = seconds / 60;
    seconds %= 60;
    minutes %= 60;

    Serial.print("\r");
    Serial.print(minutes);
    Serial.print(":");
    Serial.print(seconds);
    Serial.println(" Remaining until rec start");
    }

void Tbox::playAlert(){
    tone(_TONE_PIN,1000,500);
}

void Tbox::syncUSV(){
  Serial.print("Running Audio Tone Synchronizer...");
  tone(_TONE_PIN, 1000, 100);
  delay(350);
  tone(_TONE_PIN, 2000, 100);
  delay(350);
  tone(_TONE_PIN, 5000, 500);
  delay(750);
  Serial.println("Sync tone finished");
}