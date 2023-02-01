//boot switch
#define BOOT_SWITCH_PIN D0
#define BOOT_SWITCH_SETUP_VALUE 0
static int mode = 0;

//EEPROM
//questa struct definisce la struttura per l'eeprom
#define EEPROM_SIZE 256
typedef struct {

  char ssid[32 + 1];     //2-32
  char password[64];     //8-63
  char server_url[128];  //128
  long board_code;       //4

} dataStorage;



//questa struct definisce i dati letti dai sensori che verranno formattati per essere inviati al server
typedef struct {

  float temp_C;
  float pressure_hPA;
  float humidity_percent;
  
} sensorData;

void setup() {
  pinMode(BOOT_SWITCH_PIN, INPUT);
  //leggi il pin di boot

  mode = digitalRead(BOOT_SWITCH_PIN);

  setupStatusLed();

  //in base se è attivo o no va in modalita setup o normal
  if (mode == BOOT_SWITCH_SETUP_VALUE) {

    statusReset();
    statusSetRed(HIGH);

    setupModeSetup();

  } else {

    statusReset();
    statusSetGreen(HIGH);

    normalModeSetup();

  }
}

void loop() {
  
  if (mode == BOOT_SWITCH_SETUP_VALUE) {

    setupModeLoop();

  } else {

    normalModeLoop();

  }

}
