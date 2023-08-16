#include <Tbox.h>
Tbox tbox;
void setup() {
  tbox.begin();
  tbox.attachDefaults();

}

void loop() {
  delay(1000);
  tbox.user_wait();
  tbox.probe_settle(0.75);
  tbox.start_recording();
  tbox.open_O2();
  delay(2000);
  tbox.open_RA();
  delay(2000);
  tbox.open_HC();
  delay(2000);
  tbox.open_HO();
  delay(2000);
  tbox.open_N2();
  delay(2000);
  tbox.open_O2();
  delay(2000);
  tbox.hering_breuer(5, 500, 2000);
  tbox.stop_recording();


}
