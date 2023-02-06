#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

LiquidCrystal_I2C lcd(39, 16, 2);

const char* ssid = "Always3.1";
const char* password = "0818698877";
const char* host = "api.thingspeak.com";
unsigned long time1, time2;

WiFiClient client;
DynamicJsonDocument doc(2048);
DeserializationError error;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //lcd.init();
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello, world!");
  lcd.setCursor(1, 1);
  lcd.print("Arduino!");
  delay(1000);
  lcd.clear();

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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

  if (millis() - time1 >= 10000) {  // delay 10 sec
    Serial.println();
    time1 = millis();
    connectHost();
    String url = "/channels/2014343/feeds";
    url = url + ".json?results=";
    url = url + "1";
    Serial.print("Requesting URL: ");
    Serial.println(url);
    client.println(String("GET ") + url + " HTTP/1.1");
    client.println("Host: " + String(host));
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
    String line = "";
    while (client.available()) {
      line = line + char(client.read());
      delay(1);
    }
    delay(10);
    client.stop();
    delay(10);
    int JSfirst = line.indexOf('{');
    int JSlast = line.lastIndexOf('}');
    line.remove(JSlast + 2);
    line.remove(0, JSfirst);
    error = deserializeJson(doc, line);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    String feeds = doc["feeds"];
    feeds.replace("[", "");
    feeds.replace("]", "");

    error = deserializeJson(doc, feeds);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    String field1 = doc["field1"];
    String field2 = doc["field2"];
    Serial.print("Temp:");
    Serial.print(field1);
    Serial.print("Humid:");
    Serial.println(field2);
    lcd.setCursor(0, 0);
    lcd.print("Temp " + field1);
    lcd.setCursor(0, 1);
    lcd.print("Humid " + field2);
  }
}

int connectHost() {
  if (!client.connect(host, 80)) {
    Serial.println(" !!! Connection failed !!! ");
    delay(10);
    return 0;
  } else {
    return 1;
  }
}
