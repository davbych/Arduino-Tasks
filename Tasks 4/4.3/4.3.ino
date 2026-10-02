const int POT = A0;
const int LED = 9;

const unsigned long PRINT_INTERVAL = 200;
unsigned long lastPrint = 0;

// 2% от диапазона 0..1023
const int LOW_DEAD  = 1023 * 0.02;
const int HIGH_DEAD = 1023 - 1023 * 0.02;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int raw = analogRead(POT);
  int pwm;

  if (raw <= LOW_DEAD) {
    pwm = 0;
  } else if (raw >= HIGH_DEAD) {
    pwm = 255;
  } else {
    pwm = map(raw, LOW_DEAD, HIGH_DEAD, 1, 254);
    pwm = constrain(pwm, 0, 255);
  }

  analogWrite(LED, pwm);

  if (millis() - lastPrint >= PRINT_INTERVAL) {
    lastPrint = millis();
    Serial.print("raw=");
    Serial.print(raw);
    Serial.print("  pwm=");
    Serial.println(pwm);
  }
}