#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//pagina web per il setup della scheda
#define SETUP_PAGE_HTML "<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <title>Progetto Tpsit</title></head><style> html { height: 100%; } body { margin:0; padding:0; font-family: sans-serif; background: #FF2E63; } .login-box { position: absolute; top: 50%; left: 50%; width: 400px; padding: 40px; transform: translate(-50%, -50%); background: rgba(0,0,0,.5); box-sizing: border-box; box-shadow: 0 15px 25px rgba(0,0,0,.6); border-radius: 10px; } .login-box h2 { margin: 0 0 30px; padding: 0; color: #EAEAEA; text-align: center; } .login-box .user-box { position: relative; } .login-box .user-box input { width: 100%; padding: 10px 0; font-size: 20px; color: white; margin-bottom: 30px; border: none; border-bottom: 1px solid white; outline: none; background: transparent; } .login-box .user-box label { position: absolute; top:0; left: 0; padding: 10px 0; font-size: 20px; color: white; pointer-events: none; transition: .5s; } .login-box .user-box input:focus ~ label, .login-box .user-box input:valid ~ label { top: -20px; left: 0; color: white; font-size: 12px; } .login-box form a { position: relative; display: inline-block; padding: 10px 20px; color: #EAEAEA; font-size: 20px; text-decoration: none; text-transform: uppercase; overflow: hidden; transition: .5s; margin-top: 4px; letter-spacing: 4px } .login-box a:hover { background: rgb(37, 42, 52, 0.1); color: #EAEAEA; border-radius: 5px; box-shadow: 0 0 5px #FF2E63, 0 0 25px #FF2E63, 0 0 50px #FF2E63, 0 0 100px #FF2E63; } .login-box a span { position: absolute; display: block; } .login-box a span:nth-child(1) { top: 0; left: -100%; width: 100%; height: 2px; background: linear-gradient(90deg, transparent, #ffff00); animation: btn-anim1 1s linear infinite; } @keyframes btn-anim1 { 0% { left: -100%; } 50%,100% { left: 100%; } } .login-box a span:nth-child(2) { top: -100%; right: 0; width: 2px; height: 100%; background: linear-gradient(180deg, transparent, #d4af37); animation: btn-anim2 1s linear infinite; animation-delay: .25s } @keyframes btn-anim2 { 0% { top: -100%; } 50%,100% { top: 100%; } } .login-box a span:nth-child(3) { bottom: 0; right: -100%; width: 100%; height: 2px; background: linear-gradient(270deg, transparent, #ffff00); animation: btn-anim3 1s linear infinite; animation-delay: .5s } @keyframes btn-anim3 { 0% { right: -100%; } 50%,100% { right: 100%; } } .login-box a span:nth-child(4) { bottom: -100%; left: 0; width: 2px; height: 100%; background: linear-gradient(360deg, transparent, #d4af37); animation: btn-anim4 1s linear infinite; animation-delay: .75s } @keyframes btn-anim4 { 0% { bottom: -100%; } 50%,100% { bottom: 100%; } } #mouse:hover { cursor:pointer; }</style><body><div class=\"login-box\"> <h2>Accesso Scheda</h2> <form action=\"setup\" method=\"get\" id=\"formx\"> <div class=\"user-box\"> <input type=\"text\" name=\"nomeRete\"> <label>Nome Rete</label> </div> <div class=\"user-box\"> <input type=\"password\" name=\"passwRete\"> <label>Password Rete</label> </div> <div class=\"user-box\"> <input type=\"text\" name=\"indirizzoServer\"> <label>Indirizzo Server</label> </div> <div class=\"user-box\"> <input type=\"text\" name=\"codScheda\"> <label>codice scheda</label> </div> <a id=\"mouse\" onclick=\"document.getElementById('formx').submit()\"> <span></span> <span></span> <span></span> <span></span> Login </a> </form></div></body></html>"

#include <EEPROM.h>

ESP8266WebServer server(80);

void logRequest() {
  Serial.println("user requested a page");
}

void handleRoot() {
  logRequest();

  server.send(200, "text/html", SETUP_PAGE_HTML);
}

void handleGet() {
  logRequest();

  dataStorage ds;

  EEPROM.get(0,ds);

  Serial.println(ds.ssid);
  Serial.println(ds.password);
  Serial.println(ds.server_url);
  Serial.println(ds.board_code);

  server.send(200,"text/html","OK");
}

void handleSetup() {
  logRequest();

  int count = server.args();

  //campo obbligatorio
  String network_ssid = server.arg("nomeRete");

  //campo opzionale
  String network_pw = server.arg("passwRete");

  //campo obbligatorio
  String server_url = server.arg("indirizzoServer");

  //campo obligatorio
  String board_code_string = server.arg("codScheda");
  long board_code = board_code_string.toInt();

  if (
    network_ssid == "" | server_url == "" | board_code_string == "") {
    server.send(406, "text/html", "<h1>i parametri passati a questa pagina non sono quelli aspettati</h1>");
  }

  dataStorage ds;

  network_ssid.toCharArray(ds.ssid, 33);
  network_pw.toCharArray(ds.password, 64);
  server_url.toCharArray(ds.server_url, 128);

  ds.board_code = board_code;

  //memorizza i dati sulla eeprom
  EEPROM.begin(EEPROM_SIZE);

  EEPROM.put(0, ds);

  EEPROM.commit();

  server.send(200, "text/html", "<h1>Ok!</h1>");
}

void setupModeSetup() {

  Serial.begin(9600);
  EEPROM.begin(EEPROM_SIZE);

  Serial.println("setup mode");

  //open access point
  WiFi.softAP("ESPsoftAP_01");

  //setup server
  server.on("/", handleRoot);
  server.on("/setup", handleSetup);
  server.on("/get", handleGet);

  //start server
  server.begin();

  Serial.println("server on!");
  Serial.println("try ip:192.168.4.1");
}


void setupModeLoop() {
  //server loop
  server.handleClient();
}
