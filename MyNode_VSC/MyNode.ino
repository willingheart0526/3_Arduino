#include ".\arest_funs.h"
#include ".\dht_funs.h"

void setup() {

  // Start Serial
  Serial.begin(115200);
  
  setupDHT();
  
  setupARest();

}

void loop() {

  //  loopDHT();
  loopDHT_Fake();
  
  loopARest();
  uploadData();

}
