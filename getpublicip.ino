/* Original code modified */

#include <ESP8266HTTPClient.h>
#include <BlynkSimpleEsp8266.h>
#include "ESP8266WiFi.h"

#define BLYNK_PRINT Serial

char auth[] = ""; // Blynk Auth Tocken
char ssid[] = ""; // Wifi SSID
char pass[] = ""; // Wifi Password
char api[] = "http://bot.whatismyipaddress.com";

// Attach virtual serial terminal to Virtual Pin V0
WidgetTerminal terminal(V0);
HTTPClient http;

void print_help(){
  terminal.printf("-------------\n");
  terminal.printf("[HELP] Begin\n");
  terminal.printf("Commands:\n");
  terminal.printf("[+] /help  Print help.\n");
  terminal.printf("[+] /clear Clear terminal.\n");
  terminal.printf("[+] /wifi  Show wifi credentials.\n");
  terminal.printf("[+] /ip    Check public IP.\n");
  terminal.printf("[HELP] End\n");
  terminal.printf("-----------\n");
  terminal.flush();
}

void clear_terminal(){
  terminal.clear();
  terminal.flush();
}

void terminal_invalid_command(){
  terminal.println(F("[-] Invalid command!\n"));
  terminal.println(F("[+] Type /help to print help.\n"));
  terminal.flush();
}

void print_wifi_credentials(){
  terminal.printf("-------------\n");
  terminal.printf("[WIFI] Begin\n");
  terminal.printf("[+] SSID: %s\n", ssid );
  terminal.printf("[+] PASS: %s\n", pass );
  terminal.printf("[WIFI] End\n");
  terminal.printf("-----------\n");
  terminal.flush();
}

void update_ip()
{
  terminal.printf("-----------------\n");
  terminal.flush();
  terminal.printf("[IP] Updating...\n");
  terminal.flush();
  http.begin(api);
  
  terminal.printf("[+][GET] %s\n", api );
  int httpCode = http.GET();

  if(httpCode > 0){    
    if (httpCode == HTTP_CODE_OK) {
      terminal.printf("[+][CODE] %d : SUCCESS!\n", httpCode );
      String payload = http.getString();
      terminal.printf("[+]IP: %s\n", payload.c_str());
      Blynk.virtualWrite(V1, payload);
    }else{
      terminal.printf("[-][CODE] %d : FAILED1!\n", httpCode );
    }
  }else{
    terminal.printf("[-][CODE] %d : FAILED2!\n", httpCode );
  }
  http.end();
  terminal.printf("[IP] Updated!\n");
  terminal.printf("--------------\n");
  terminal.flush();
}

BLYNK_WRITE(V2) // HELP
{
  // Call print_help function when button is pressed on Virtual Pin V2.
  if(param.asInt() == 1){
    print_help();
  }
}

BLYNK_WRITE(V3) // CLEAR
{
  // Call clear_terminal function when button is pressed on Virtual Pin V3.
  if(param.asInt() == 1){
    clear_terminal();
  }
}

BLYNK_WRITE(V5) // SHOW IP
{
  // Call update_ip function when button is pressed on Virtual Pin V5.
  if(param.asInt() == 1){
    update_ip();
  }
}

BLYNK_WRITE(V0)
{
  if(String("/help").equalsIgnoreCase(param.asStr())) {
    print_help();
  }else if(String("/clear").equalsIgnoreCase(param.asStr())){
    clear_terminal();
  }else if(String("/wifi").equalsIgnoreCase(param.asStr())){
    print_wifi_credentials();
  }else if(String("/ip").equalsIgnoreCase(param.asStr())){
    update_ip();
  }else{
    terminal_invalid_command();
  }
}

void setup()
{
  Blynk.begin(auth, ssid, pass);
  terminal.println(F("[Blynk v" BLYNK_VERSION "] Device started"));
  terminal.println(F("-------------------------------"));
  terminal.flush();
  print_help();
  print_wifi_credentials();
  Blynk.virtualWrite(V10, api);
  terminal.printf("API is set to %s \n", api );
  terminal.flush();
}

void loop()
{
  Blynk.run();
}
