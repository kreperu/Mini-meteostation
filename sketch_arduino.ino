#include <Adafruit_BMP280.h>

#include <ESP8266WiFi.h>
#include <math.h>

WiFiClient client;
Adafruit_BMP280 bmp;

#define STASSID "SSID"
#define STAPSK "PASSWORD"
#define CITY "CITY"

#define SERVER "192.168.1.70"

#define fori(len) for(int i = 0; i < len; i++)

const char* ssid = STASSID;
const char* password = STAPSK;
const char* server = SERVER;

int data;

static inline uint8_t toDec(const char* hex) {
  return ((isdigit(hex[0])?(hex[0]-48):(toupper(hex[0])-55))<<4)+(isdigit(hex[1])?(hex[1]-48):(toupper(hex[1])-55));
}

void setup() {
  //Serial.begin(115200);
  bmp.begin();
  //Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    //Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  const char* mac = WiFi.macAddress().c_str();
  uint8_t macNum[6];
  fori(6){
    char twoDigit[] = {mac[i*3], mac[i*3+1]};
    Serial.println(twoDigit);
    macNum[i] = toDec(twoDigit);
    Serial.println(macNum[i]);
  }
  //Station ID
  uint32_t idNum = macNum[3] << 16 + macNum[4] << 8 + macNum[5];
  Serial.println(idNum);
  
  if(client.connect(server, 8000)) {
    //TODO Finish sending
    client.println("POST  HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("User-Agent: MiniMeteostat/0.1 (ESP-12E; Arduino)");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.print("[{\"id_stacji\":\"");
    client.print(idNum);
    client.print("\",\"stacja\":\"");
    client.print(CITY);
    client.print("\",\"data_pomiaru\":\"");
    } else {
        while(1) {
          Serial.println("NOT  Connected");
          delay(100);
       }
   }
}

void loop() {
}
