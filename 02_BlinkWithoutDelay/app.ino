#define LED_BUILTIN 2

int ledState = LOW;     

unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;   
    if (ledState == LOW)
      ledState = HIGH;  // Note that this switches the LED *off*
    else
      ledState = LOW;   // Note that this switches the LED *on*
    digitalWrite(LED_BUILTIN, ledState);
  }
}
