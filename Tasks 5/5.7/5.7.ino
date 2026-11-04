#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int PHOTO = A0;
const int PIN_R = 9;
const int PIN_G = 10;
const int PIN_B = 11;

const unsigned long PHOTO_INTERVAL = 200;
const unsigned long DHT_INTERVAL = 2000;

const int LIGHT_DARK = 200;
const int LIGHT_BRIGHT = 600;
const int HYST = 50;

const float TEMP_LOW = 18.0;
const float TEMP_HIGH = 26.0;
const float TEMP_HYST = 0.5;

unsigned long lastPhoto = 0;
unsigned long lastDht = 0;

int lightValue = 0;
float tempValue = 0.0;
bool hasTemp = false;

int category = -1;

int classifyLight(int v) {
  static int lastLightCat = -1;
  int cat;
  if (v < LIGHT_DARK) cat = 0;
  else if (v > LIGHT_BRIGHT) cat = 2;
  else cat = 1;

  if (lastLightCat == 0 && v > LIGHT_DARK + HYST) lastLightCat = 1;
  else if (lastLightCat == 1 && v < LIGHT_DARK - HYST) lastLightCat = 0;
  else if (lastLightCat == 1 && v > LIGHT_BRIGHT + HYST) lastLightCat = 2;
  else if (lastLightCat == 2 && v < LIGHT_BRIGHT - HYST) lastLightCat = 1;
  else if (lastLightCat < 0) lastLightCat = cat;

  return lastLightCat;
}

int classifyTemp(float t) {
  static int lastTempCat = -1;
  int cat;
  if (t < TEMP_LOW) cat = 0;
  else if (t > TEMP_HIGH) cat = 2;
  else cat = 1;

  if (lastTempCat == 0 && t > TEMP_LOW + TEMP_HYST) lastTempCat = 1;
  else if (lastTempCat == 1 && t < TEMP_LOW - TEMP_HYST) lastTempCat = 0;
  else if (lastTempCat == 1 && t > TEMP_HIGH + TEMP_HYST) lastTempCat = 2;
  else if (lastTempCat == 2 && t < TEMP_HIGH - TEMP_HYST) lastTempCat = 1;
  else if (lastTempCat < 0) lastTempCat = cat;

  return lastTempCat;
}

void setColor(int r, int g, int b) {
  digitalWrite(PIN_R, r);
  digitalWrite(PIN_G, g);
  digitalWrite(PIN_B, b);
}

const char* catName(int c) {
  switch (c) {
    case 0: return "DARK/COLD";
    case 1: return "NORMAL";
    case 2: return "BRIGHT";
    case 3: return "HOT";
    default: return "?";
  }
}

void loop() {
  unsigned long now = millis();

  if (now - lastPhoto >= PHOTO_INTERVAL) {
    lastPhoto = now;
    lightValue = analogRead(PHOTO);
  }

  if (now - lastDht >= DHT_INTERVAL) {
    lastDht = now;
    float t = dht.readTemperature();
    if (!isnan(t)) {
      tempValue = t;
      hasTemp = true;
    }
  }

  if (!hasTemp) return;

  int lc = classifyLight(lightValue);
  int tc = classifyTemp(tempValue);

  int newCat;
  if (lc == 0 && tc == 0) newCat = 0;
  else if (lc == 2 && tc == 2) newCat = 3;
  else if (lc == 1 && tc == 1) newCat = 1;
  else newCat = 2;

  if (newCat != category) {
    category = newCat;
    Serial.print("t=");
    Serial.print(millis());
    Serial.print(" light=");
    Serial.print(lightValue);
    Serial.print(" temp=");
    Serial.print(tempValue, 1);
    Serial.print(" cat=");
    Serial.println(catName(category));
  }

  if (category == 0) setColor(0, 0, 1);
  else if (category == 1) setColor(0, 1, 0);
  else if (category == 2) setColor(1, 1, 0);
  else setColor(1, 0, 0);
}