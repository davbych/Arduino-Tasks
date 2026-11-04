#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const int PIN_R = 9;
const int PIN_G = 10;
const int PIN_B = 11;

const unsigned long INTERVAL = 2000;

unsigned long lastRead = 0;
float lastT = 0.0;
float lastH = 0.0;
bool hasValid = false;

int okCount = 0;
int failCount = 0;
int totalCount = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
}

void setColor(bool r, bool g, bool b) {
  digitalWrite(PIN_R, r ? HIGH : LOW);
  digitalWrite(PIN_G, g ? HIGH : LOW);
  digitalWrite(PIN_B, b ? HIGH : LOW);
}

void loop() {
  if (millis() - lastRead < INTERVAL) return;
  lastRead = millis();

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  totalCount++;

  if (isnan(t) || isnan(h)) {
    failCount++;
    Serial.println("DHT read error");
    if (hasValid) {
      t = lastT;
      h = lastH;
    } else {
      return;
    }
  } else {
    okCount++;
    lastT = t;
    lastH = h;
    hasValid = true;
  }

  bool tOk = (t >= 20.0 && t <= 25.0);
  bool hOk = (h >= 40.0 && h <= 60.0);

  if (tOk && hOk) {
    setColor(false, true, false);
  } else if (!tOk && !hOk) {
    setColor(true, false, false);
  } else {
    setColor(true, true, false);
  }

  Serial.print("T=");
  Serial.print(t, 1);
  Serial.print(" H=");
  Serial.print(h, 1);

  if (totalCount % 20 == 0) {
    Serial.print(" | stats: ok=");
    Serial.print(okCount);
    Serial.print(" fail=");
    Serial.print(failCount);
    Serial.print(" total=");
    Serial.print(totalCount);
  }
  Serial.println();
}