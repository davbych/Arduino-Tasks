#include <DHT.h>

const int LM35_PIN = A0;
const int THERM_PIN = A1;
const int DHT_PIN = 2;

DHT dht(DHT_PIN, DHT11);

const float THERM_R0 = 10000.0;
const float THERM_B = 3950.0;
const float THERM_T0 = 298.15;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

float readLM35() {
  int v = analogRead(LM35_PIN);
  return v * 5.0 / 1024.0 * 100.0;
}

float readThermistor() {
  int v = analogRead(THERM_PIN);
  float r = 10000.0 * (1023.0 / v - 1.0);
  float t = 1.0 / (1.0 / THERM_T0 + log(r / THERM_R0) / THERM_B);
  return t - 273.15;
}

void loop() {
  float t1 = readLM35();
  float t2 = readThermistor();
  float t3 = dht.readTemperature();

  if (isnan(t3)) t3 = -999;

  Serial.print(t1, 1);
  Serial.print(",");
  Serial.print(t2, 1);
  Serial.print(",");
  Serial.println(t3, 1);

  delay(2000);
}