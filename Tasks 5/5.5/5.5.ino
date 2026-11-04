const int JOY_X = A0;
const int JOY_Y = A1;
const int JOY_BTN = 2;
const int LED_X = 9;
const int LED_Y = 10;

const int DEAD_ZONE = 10;
const int BTN_DEBOUNCE = 50;

int centerX = 0;
int centerY = 0;

bool mode = 0;
bool lastBtn = HIGH;
unsigned long btnChangeTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_X, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(JOY_BTN, INPUT_PULLUP);

  long sx = 0, sy = 0;
  for (int i = 0; i < 50; i++) {
    sx += analogRead(JOY_X);
    sy += analogRead(JOY_Y);
    delay(5);
  }
  centerX = sx / 50;
  centerY = sy / 50;

  Serial.print("centerX=");
  Serial.print(centerX);
  Serial.print(" centerY=");
  Serial.println(centerY);
}

int readAxis(int pin, int center) {
  int v = analogRead(pin) - center;
  if (abs(v) < DEAD_ZONE) return 0;
  int out = v * 100 / 512;
  if (out > 100) out = 100;
  if (out < -100) out = -100;
  return out;
}

const char* direction(int x, int y) {
  int ax = abs(x);
  int ay = abs(y);
  if (ax < DEAD_ZONE && ay < DEAD_ZONE) return "C";
  if (ay > ax * 2) return (y > 0) ? "S" : "N";
  if (ax > ay * 2) return (x > 0) ? "E" : "W";
  if (x > 0 && y > 0) return "SE";
  if (x > 0 && y < 0) return "NE";
  if (x < 0 && y > 0) return "SW";
  return "NW";
}

void loop() {
  bool btn = digitalRead(JOY_BTN);
  if (btn != lastBtn) {
    btnChangeTime = millis();
    lastBtn = btn;
  }
  if (millis() - btnChangeTime > BTN_DEBOUNCE) {
    static bool prev = HIGH;
    if (btn == LOW && prev == HIGH) {
      mode = !mode;
    }
    prev = btn;
  }

  int x = readAxis(JOY_X, centerX);
  int y = readAxis(JOY_Y, centerY);

  int ax = abs(x);
  int ay = abs(y);

  if (mode == 0) {
    analogWrite(LED_X, map(ax, 0, 100, 0, 255));
    analogWrite(LED_Y, map(ay, 0, 100, 0, 255));
  } else {
    unsigned long period = map(max(ax, ay), 0, 100, 1000, 50);
    bool on = (millis() % period) < (period / 2);
    analogWrite(LED_X, on ? map(ax, 0, 100, 0, 255) : 0);
    analogWrite(LED_Y, on ? map(ay, 0, 100, 0, 255) : 0);
  }

  Serial.print("dir=");
  Serial.print(direction(x, y));
  Serial.print(" x=");
  Serial.print(x);
  Serial.print(" y=");
  Serial.print(y);
  Serial.print(" mode=");
  Serial.println(mode ? "blink" : "bright");

  delay(50);
}