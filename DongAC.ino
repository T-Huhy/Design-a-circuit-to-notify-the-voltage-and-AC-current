#define BLYNK_TEMPLATE_ID "TMPLiNvXP8uL"
#define BLYNK_DEVICE_NAME "Voltage Current"
#define BLYNK_PRINT Serial

#include "EmonLib.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

EnergyMonitor emon;
#define vCalibration 220
#define currCalibration 1.5
BlynkTimer timer;

#define BLYNK_AUTH_TOKEN "cs8jwRC1qeVWs3mfdR4jPkUB-mGqnnLK"

char ssid[] = "HUYHOANG";
char pass[] = "0908756975";

float kWh = 0;
unsigned long lastmillis = millis();

void myTimerEvent() {
  emon.calcVI(20, 2000);
  Serial.print("Vrms: ");
  Serial.print(emon.Vrms, 2);
  Serial.print("V");
  Blynk.virtualWrite(V0, emon.Vrms);
  Serial.print("\tIrms: ");
  Serial.print(emon.Irms, 4);
  Serial.print("A");
  Blynk.virtualWrite(V1, emon.Irms);
  Serial.print("\tPower: ");
  Serial.print(emon.apparentPower, 4);
  Serial.print("W");
  Blynk.virtualWrite(V2, emon.apparentPower);
  Serial.print("\tkWh: ");
  kWh = kWh + emon.apparentPower * (millis() - lastmillis) / 3600000000.0;
  Serial.print(kWh, 4);
  Serial.println("kWh");
  lastmillis = millis();
  Blynk.virtualWrite(V3, kWh);
    if (emon.Vrms>230||emon.Vrms<210||emon.Irms>0.7||emon.Irms<0.4)
  {
      char* caution = {"Nguy hiểm"};
     Serial.println(caution);
     Blynk.virtualWrite(V4, caution);
  }
}

void setup() {
  Serial.begin(9600);
  emon.voltage(35, vCalibration, 1.7); // Voltage: input pin, calibration, phase_shift
  float emon.Vrms = random(21900,22100)/100;
  emon.current(34, currCalibration); // Current: input pin, calibration.
  float emon.Irms = random(120,150)/100;
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(5000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();
}
