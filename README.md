# ESP8266_AIRQUALITY

il progetto contiene 4 file:

- esp8266project.ino
- normalMode.ino
- setupMode.ino
- statusLed.ino
  
---

## esp8266project

qui c'è il check del boot switch a inizio esecuzione

### possibili modifiche:

- in questo momento il programma fa un check del valore `mode` a ogni iterazione della funzione loop e quindi si potrebbe usare i puntatori di funzione per evitare il check

---

## normalMode

in normal mode la scheda chiede al server ogni n secondi se ha bisogno di mandare dati al database

c'è rimasto da integrare i sensori nella struct `sensorData`

---

## setupMode

invece in setup mode al scheda diventa un access point dove attraverso una paghina web si possono immettere i dati di una scheda.

### possibili modifiche:

- quando un dispositivo si connette al AP mostrare direttamente la pagina di setup invece di cercare `192.168.4.1`
- c'è anche da rifare o modificare la pagina di setup che ha diversi problemi con i campi ad esempio quello della password del wifi è situata nel `#define SETUP_PAGE_HTML`

---

## statusLed

inseriti qui ci sono tutte le funzioni che attuano il led rgb collegato sulla scheda

### possibili modifiche:

- indicare nella segnatura se è una funzione bloccante o no