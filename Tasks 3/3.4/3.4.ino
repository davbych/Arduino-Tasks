const int LEDS[] = {3, 4, 5, 6, 7, 8};
const int LED_COUNT = sizeof(LEDS) / sizeof(LEDS[0]);

const int BUTTON = 2;
const unsigned long DEBOUNCE = 50;
const int STEP = 80;

int effect = 0;
bool raw = HIGH, last = HIGH, stable = HIGH;
unsigned long changeTime = 0;

void setup() {
  for (int i = 0; i < LED_COUNT; i++) pinMode(LEDS[i], OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("effect 0");
}

void allOff() {
  for (int i = 0; i < LED_COUNT; i++) digitalWrite(LEDS[i], LOW);
}

void effectRunner() {
  for (int i = 0; i < LED_COUNT; i++) {
    allOff();
    digitalWrite(LEDS[i], HIGH);
    delay(STEP);
  }
}

void effectCounter() {
  for (int i = 0; i < LED_COUNT / 2; i++) {
    allOff();
    digitalWrite(LEDS[i], HIGH);
    digitalWrite(LEDS[LED_COUNT - 1 - i], HIGH);
    delay(STEP);
  }
  allOff();
  delay(STEP);
}

void effectFill() {
  for (int i = 0; i < LED_COUNT; i++) {
    digitalWrite(LEDS[i], HIGH);
    delay(STEP);
  }
  for (int i = 0; i < LED_COUNT; i++) {
    digitalWrite(LEDS[i], LOW);
    delay(STEP);
  }
}

void effectRandom() {
  allOff();
  digitalWrite(LEDS[random(LED_COUNT)], HIGH);
  delay(STEP);
}

void loop() {
  unsigned long now = millis();

  raw = digitalRead(BUTTON);
  if (raw != last) changeTime = now;

  if (now - changeTime > DEBOUNCE && raw != stable) {
    stable = raw;
    if (stable == LOW) {
      effect = (effect + 1) % 4;   // 4 эффекта, по кругу
      allOff();
      Serial.print("effect ");
      Serial.println(effect);
    }
  }
  last = raw;

  switch (effect) {
    case 0: effectRunner();  break;
    case 1: effectCounter(); break;
    case 2: effectFill();    break;
    case 3: effectRandom();  break;
  }
}
