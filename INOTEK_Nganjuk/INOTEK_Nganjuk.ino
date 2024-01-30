#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_TEMPLATE_ID "TMPL6ZQrifThu"
#define BLYNK_TEMPLATE_NAME "Inotek 2023"
#define BLYNK_AUTH_TOKEN "W-yYyN4uSao487091rXJcRXwlWLaB-Jv"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "admin1";
char pass[] = "123456789";

BlynkTimer timer;

#include <MQUnifiedsensor.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define placa "ESP-32"
#define Voltage_Resolution 3.3
#define pin 32
#define type "MQ-5"
#define ADC_Bit_Resolution 12
#define RatioMQ5CleanAir 6.5  //RS / R0 = 6.5 ppm

#define fr 27
#define buzzer 26
#define fan1 2
#define fan2 4

bool frValue;
String statusFr;
float mqValue;

//Millis
unsigned long interval = 1000;
unsigned long previousMillis = 0;

MQUnifiedsensor MQ5(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  initWiFi();
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  mqBegin();
  pinMode(buzzer, OUTPUT);
  pinMode(fan1, OUTPUT);
  pinMode(fan2, OUTPUT);
  pinMode(fr, INPUT);
}

void loop() {
  Blynk.run();
  run();
}