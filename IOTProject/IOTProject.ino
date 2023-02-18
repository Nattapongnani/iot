//Tech Trends Shameer
#define BLYNK_TEMPLATE_ID "TMPLk4gWG73R"
#define BLYNK_TEMPLATE_NAME "Smart Fan"
#define BLYNK_AUTH_TOKEN "dtmarKO_GpA4bl0pvvCjqbPQWUIVGv_7"

const int TRIGGER_PIN = 5;
const int ECHO_PIN = 17;
int Relay = 12;

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
long duration, distance;

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Always3.2";   // type your wifi name
char pass[] = "0818698877";  // type your wifi password

BlynkTimer timer;

int DisSetting = 6;
int TimeSetting = 0;
int t;
bool startCount = false;
int Timeout;

void setup() {
  Serial.begin(9600);
  pinMode(Relay, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(500L, sendSensor);
  timer.setInterval(1000L, countCharger1);
}

void loop() {
  Blynk.run();
  timer.run();
}
int i=0;
void sendSensor() {

  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  // คำนวนระยะ หน่วย CM
  distance = (duration / 2) / 29.1;

  //  Display distance, distance_Setting
  
  Serial.print("    distance : ");
  Serial.print(distance);
  Serial.print(" cm");

  Serial.print("    distance_Setting : ");
  Serial.print(DisSetting);
  Serial.println(" cm");
  
  
  if (distance <= DisSetting) {
    
    startCount = true;
    if(i == 0){
      //On
      digitalWrite(Relay, HIGH);
      Blynk.virtualWrite(V2, 1);  //Turn On Fan
      i=1;      
    }else{
      //Off
      digitalWrite(Relay, LOW);
      Blynk.virtualWrite(V2, 0);  //Turn On Fan
      i=0;  
    }
  } 
}

/***************************************************************************/
BLYNK_WRITE(V3) { // Time Settings
  TimeSetting = param.asInt();
  t = TimeSetting;
  Blynk.virtualWrite(V4, t); //Timeout
}

BLYNK_WRITE(V2) {  //Relay
  if (param.asInt() == 1) {
    startCount = true;
    digitalWrite(Relay, HIGH);
  } else {
    digitalWrite(Relay, LOW);
    startCount = false;
  }
}

/***************************************************************************/

void countCharger1() {
  if (t >= 0 && startCount == true) {
    Blynk.virtualWrite(V4, t);
    if(t == 0){
      Blynk.virtualWrite(V2, 0);
      digitalWrite(Relay, LOW);
      Blynk.virtualWrite(V3, 0);
      startCount = false;
    }
    t--;
  }
}
