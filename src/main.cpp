#include <Arduino.h>
#include <WiFi.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

const char* ssid = "Ambu";
const char* password = "LANCE001";

const char* wifi_network_ssid = "Beeline_D0CF";
const char* wifi_network_password =  "76287572";

#define BOTtoken "1920827174:AAE1BGQGhKYZ0m6nKGw_gB3DFFhBNd_hjss"
#define CHAT_ID "497344647"

WiFiClientSecure clientSecure;
UniversalTelegramBot bot(BOTtoken, clientSecure);

String header; 
WiFiServer server(80);

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.softAP(ssid, password);
  WiFi.begin(wifi_network_ssid, wifi_network_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.print("ESP32 IP as soft AP: ");
  Serial.println(WiFi.softAPIP());
  
  Serial.print("ESP32 IP on the WiFi network: ");
  Serial.println(WiFi.localIP());
  
  clientSecure.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  server.begin(); 
}

void loop() {
  WiFiClient client = server.available(); 
  int NEWCLI = 0;
  while(client.connected()){ 
    NEWCLI = 1;
    if (client.available()) {
      char c = client.read();
      header += c;
    }
  }
  if(NEWCLI == 1){
    Serial.print("UserID: ");

    int last = header.indexOf("HTTP/1.1");
    String numbers = header.substring(5, last - 1);
    Serial.println(numbers);
    bot.sendMessage(CHAT_ID, numbers);

    header = "";

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  
}

