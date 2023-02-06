//#include <ESP8266WiFi.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(39, 16, 2);

#define DHTPIN 4
//#define DHTTYPE DHT11 // DHT 11
#define DHTTYPE DHT22  // DHT 22

const char* ssid = "Always3.1";
const char* password = "0818698877";
const char* host1 = "api.thingspeak.com";
const char* api_key = "P1KA5YJ8X7NYA6Y3";
const char* api_keyRead = "7EO3Z3R6X1FUMQVG";

const int channel_id = 2014343;

unsigned long time1 = 0, time2 = 0;
float h, t;
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  dht.begin();
  lcd.begin();

  lcd.setCursor(0, 0);
  lcd.print("Temp");
  //lcd.setCursor(10, 0);
  //lcd.print(t);
  lcd.setCursor(15, 0);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity");
  //lcd.setCursor(10, 1);
  //lcd.print(h);
  lcd.setCursor(15,1);
  lcd.print("%");

  Serial.print("Connection to ");
  Serial.println(ssid);
  WiFi.disconnect();
  delay(1000);
  WiFi.mode(WIFI_STA);
  delay(1000);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address");
  Serial.println(WiFi.localIP());

  Serial.println("");
  Serial.println("");
}
/*
void temps() {
  lcd.setCursor(10,0);
  lcd.print("t");
  lcd.setCursor(10,1);
  lcd.print("h");  
}
*/

void loop() {
  
  // put your main code here, to run repeatedly:
  h = dht.readHumidity();
  t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor");
    delay(1000);
    return;
  }

 //temps();

  if (millis() - time1 >= 20000) {  //delay 20 sec
    time1 = millis();
    connectHost();

    String url = "/update?api_key=";
    url = url + api_key;
    url = url + "&field1=";
    url = url + String(t);
    url = url + "&field2=";
    url = url + String(h);
    Serial.print("Requesting URL: ");
    Serial.println(url);

    Serial.println(String("GET ") + url + " HTTP/1.1");
    Serial.println("Host: " + String(host1));
    Serial.println("Connection: close");
    Serial.println();
    delay(1000);

    client.println(String("GET ") + url + " HTTP/1.1");
    client.println("Host: " + String(host1));
    client.println("Connection: close");
    client.println();

    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    //Serial.println();
  }

}

int connectHost() {
  if (!client.connect(host1, 80, 5000)) {
    Serial.println(" !!! Connection failed !!!");
    delay(500);
    return 0;
  } else {
    return 1;
  }
}
