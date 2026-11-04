const int PHOTO = A0;
const int BUTTON = 2;
const int LEDS[] = {3, 4, 5, 6, 7, 8, 9, 10};
const int N = 8;

int calMin = 1023;
int calMax = 0;

float ema = 0;
bool emaInit = false;

const unsigned long CAL_TIME = 10000;
const unsigned long DEBOUNCE = 50;
const unsigned long LONG_PRESS = 1000;

bool lastBtn = HIGH;
unsigned long btnChangeTime = 0;
unsigned long pressStart = 0;
bool pressing = false;

void setLeds(int lit) {
  for (int i = 0; i < N; i++) {
    digitalWrite(LEDS[i], i < lit ? HIGH : LOW);
  }
}

void calibrate() {
  Serial.println("Calibrating 10s...");
  calMin = 1023;
  calMax = 0;
  unsigned long start = millis();
  while (millis() - start < CAL_TIME) {
    int v = analogRead(PHOTO);
    if (v < calMin) calMin = v;
    if (v > calMax) calMax = v;
    delay(10);
  }
  Serial.print("calMin=");
  Serial.print(calMin);
  Serial.print(" calMax=");
  Serial.println(calMax);
  emaInit = false;
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP);
  for (int i = 0; i < N; i++) pinMode(LEDS[i], OUTPUT);
  calibrate();
}

void loop() {
  bool btn = digitalRead(BUTTON);

  if (btn != lastBtn) {
    btnChangeTime = millis();
    lastBtn = btn;
  }

  if (millis() - btnChangeTime > DEBOUNCE) {
    if (btn == LOW && !pressing) {
      pressing = true;
      pressStart = millis();
    } else if (btn == HIGH && pressing) {
      pressing = false;
      if (millis() - pressStart < LONG_PRESS) {
        // короткое нажатие - ничего
      }
    }
  }

  if (pressing && millis() - pressStart >= LONG_PRESS) {
    pressing = false;
    calibrate();
  }

  int raw = analogRead(PHOTO);

  if (!emaInit) {
    ema = raw;
    emaInit = true;
  } else {
    ema = 0.1 * raw + 0.9 * ema;
  }

  int percent = map((int)ema, calMin, calMax, 0, 100);
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;

  int lit = map(percent, 0, 100, 0, N);
  setLeds(lit);

  Serial.print("light=");
  Serial.print(percent);
  Serial.println("%");

  delay(100);
}