const int SENSOR_POWER = 7;
const int SENSOR_SIGNAL = A0;
const int BUZZER = 12;
const int LEDS[] = {3, 4, 5, 6, 8, 9, 10, 11};
const int N = 8;

const int CRITICAL = 100;
const unsigned long INTERVAL = 5000;
const int SETTLE_MS = 10;

unsigned long lastRead = 0;
int level = 0;

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_POWER, OUTPUT);
  digitalWrite(SENSOR_POWER, LOW);
  pinMode(BUZZER, OUTPUT);
  for (int i = 0; i < N; i++) pinMode(LEDS[i], OUTPUT);
}

int readLevel() {
  digitalWrite(SENSOR_POWER, HIGH);
  delay(SETTLE_MS);
  int v = analogRead(SENSOR_SIGNAL);
  digitalWrite(SENSOR_POWER, LOW);
  return v;
}

void setLeds(int lit) {
  for (int i = 0; i < N; i++) {
    digitalWrite(LEDS[i], i < lit ? HIGH : LOW);
  }
}

void loop() {
  if (millis() - lastRead < INTERVAL) return;
  lastRead = millis();

  level = readLevel();
  int lit = map(level, 0, 1023, 0, N);
  if (lit < 0) lit = 0;
  if (lit > N) lit = N;
  setLeds(lit);

  if (level < CRITICAL) {
    digitalWrite(BUZZER, HIGH);
  } else {
    digitalWrite(BUZZER, LOW);
  }

  Serial.print("level=");
  Serial.print(level);
  Serial.print(" lit=");
  Serial.print(lit);
  Serial.print(" alarm=");
  Serial.println(level < CRITICAL ? "ON" : "OFF");
}