
#define STATUS_LED_RED_PIN D3
#define STATUS_LED_GREEN_PIN D4
#define STATUS_LED_BLUE_PIN D5

void setupStatusLed() {
  //setup pin
  pinMode(STATUS_LED_RED_PIN, OUTPUT);
  pinMode(STATUS_LED_GREEN_PIN, OUTPUT);
  pinMode(STATUS_LED_BLUE_PIN, OUTPUT);

  //initial value
  digitalWrite(STATUS_LED_RED_PIN, LOW);
  digitalWrite(STATUS_LED_GREEN_PIN, LOW);
  digitalWrite(STATUS_LED_BLUE_PIN, LOW);
}

void statusReset() {
  digitalWrite(STATUS_LED_RED_PIN, LOW);
  digitalWrite(STATUS_LED_GREEN_PIN, LOW);
  digitalWrite(STATUS_LED_BLUE_PIN, LOW);
}

void statusSetRed(uint8_t val) {

  digitalWrite(STATUS_LED_RED_PIN, val);
}

void statusSetGreen(uint8_t val) {

  digitalWrite(STATUS_LED_GREEN_PIN, val);
}

void statusSetBlue(uint8_t val) {

  digitalWrite(STATUS_LED_BLUE_PIN, val);
}

void statusErrorSensorNotDetected() {

  statusReset();
  statusSetBlue(HIGH);

  unsigned long time = millis();

  int period = 1000; 

  statusSetGreen(time % period > period / 2);
}

void statusWiFiSearch() {

  unsigned long time = millis();

  int period = 1000; 

  statusSetRed(time % period > period / 2);

}

void statusWifiError() {
  statusReset();
  statusSetBlue(HIGH);

  while (true) {
    statusSetRed(HIGH);
    delay(500);
    statusSetRed(LOW);
    delay(500);
  }
}
