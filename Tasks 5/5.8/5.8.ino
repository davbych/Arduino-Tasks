const int SENSOR_POWER = 7;
const int SENSOR_SIGNAL = A0;

const int AIR_VALUE = 1023;
const int WATER_VALUE = 300;

const unsigned long INTERVAL = 5000;
const int SETTLE_MS = 10;

unsigned long lastRead = 0;
int rawValue = 0;
int percent = 0;

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_POWER, OUTPUT);
  digitalWrite(SENSOR_POWER, LOW);
}

int readSensor() {
  digitalWrite(SENSOR_POWER, HIGH);
  delay(SETTLE_MS);
  int v = analogRead(SENSOR_SIGNAL);
  digitalWrite(SENSOR_POWER, LOW);
  return v;
}

void loop() {
  if (millis() - lastRead < INTERVAL) return;
  lastRead = millis();

  rawValue = readSensor();

  percent = map(rawValue, AIR_VALUE, WATER_VALUE, 0, 100);
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;

  Serial.print("raw=");
  Serial.print(rawValue);
  Serial.print(" percent=");
  Serial.print(percent);
  Serial.println("%");
}