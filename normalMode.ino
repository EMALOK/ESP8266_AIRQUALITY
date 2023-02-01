#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>


#include <EEPROM.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define TIME_BETWEEN_CHECK_MS 500

//Sensors
Adafruit_BME280 bme;

//EEPROM struct
dataStorage ds;

//data from sensors
sensorData sens_data;

//time from server check
unsigned long last_time = 0;
unsigned long curr_time = 0;

void normalModeSetup() {

  //apri la seriale
  Serial.begin(9600);
  Serial.println("normal mode");

  //inizializza la eeprom
  EEPROM.begin(EEPROM_SIZE);
  //leggi i contenuti dellz eeprom e mettili in ds
  ds = EEPROM.get(0, ds);

  //debug log dei dati
  Serial.println(ds.ssid);
  Serial.println(ds.password);
  Serial.println(ds.server_url);
  Serial.println(ds.board_code);

  //inizializza il wifi con il ssid e pw che sono contenuti nell'eeeprom
  WiFi.begin(ds.ssid, ds.password);

  Serial.print("connecting to wifi...");

  wl_status_t WiFi_status;

  //aspetta finche non ci si connette al wifi
  do {

    WiFi_status = WiFi.status();
    // Serial.println(WiFi_status);
    switch (WiFi_status) {
      case WL_CONNECT_FAILED:
      case WL_CONNECTION_LOST:
      case WL_WRONG_PASSWORD:
      case WL_NO_SSID_AVAIL:
        statusWifiError();  //se questa funzione viene chiamata blocca il processo e lampeggia
        break;
      case WL_DISCONNECTED:
        statusWiFiSearch();
    }
    yield();

  } while (WiFi_status != WL_CONNECTED);

  Serial.println("CONNECTED");


  //inizio della connesione con i diversi sensori
  Wire.begin();

  Serial.print("connecting to sensors...");
  uint8_t status;
  do {
    //status = bme.begin(0x77);  //OK
    Wire.beginTransmission(0x77);
    status = Wire.endTransmission();
    yield();
    if (!status) {
      statusErrorSensorNotDetected();  //while inside the sensor is not connectd
      //Serial.println(status);
    }
  } while (!status);

  Serial.println("CONNECTED");
  //fin connesione con i sensori
}

void normalModeLoop() {

  curr_time = millis();

  if (curr_time - last_time > TIME_BETWEEN_CHECK_MS) {
    //controlla che il teempo passato dal ultimo check con il server sia maggiore di TIME_BETWEEN_CHECK_MS
    //update last time
    last_time = curr_time;

    //ottieni dal server se ho bisogno di mandare le informazioni dei sensori
    if (getWork()) {

      //leggi i dati dei sensori
      readSensorData();

      //manda i dati al server
      if (sendData() != 0) {
        //controlla che sia andato a buon fine
      }
    }
  }
}

void readSensorData() {

  //questa funzione deve fare due cose differenti
  //1. leggere i dati dai sensori e convertirli nell'unità di misura corretta
  //2. metterli nella struct sensorData sens_data per poi essere mandati al server

  /*
  sens_data.temp_C = bme.readTemperature();
  sens_data.humidity_percent = bme.readHumidity();
  sens_data.pressure_hPA = bme.readPressure() / 100.0F;
  */
  Wire.beginTransmission(0x77);
  Wire.requestFrom(0x77, 1);
  while (Wire.available()) {
    int c = Wire.read();  // receive a byte as character
    Serial.printf("data: (%c)\n", c);
  }
  Wire.endTransmission();
}

bool getWork() {
  //api endpoint

  /*

  ottieni il lavoro
  
  GET /api/boards/{id}/getwork
  
  */

  WiFiClient client;
  HTTPClient http;

  String query = String(ds.server_url) + "/api/boards/" + String(ds.board_code) + "/getwork/";
  Serial.println(query);
  //                212.237.3.212

  //example:
  //212.237.3.212/api/boards/1/getwork/

  // Your Domain name with URL path or IP address with path
  http.begin(client, query.c_str());

  // If you need Node-RED/server authentication, insert user and password below
  //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

  // Send HTTP GET request
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);

    // questo è da cambiare con un check della risposta
    // return (int)payload[0] - '0';
    // questo converte '1' -> true
    //                 '0' -> false
    return (int)payload[0] - '0';

  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();


  //test
  return false;
}

//return 0 in normal operation
int sendData() {

  /*
  
  invia i dati

  POST /api/boards/{id}/data
  
  */


  //api endpoint
}
