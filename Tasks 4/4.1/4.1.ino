const int LED = 9;
const int BUTTON = 2;

bool gammaMode = false;
int brightness = 0;
int fadeAmount = 5;

uint8_t gammaCorrect(uint8_t value) {
  return (uint16_t)value * value / 255;
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(BUTTON) == LOW) {
    gammaMode = !gammaMode;
    Serial.println(gammaMode ? "gamma" : "linear");
    delay(300);
  }

  if (gammaMode) {
    analogWrite(LED, gammaCorrect(brightness));
  } else {
    analogWrite(LED, brightness);
  }

  brightness += fadeAmount;
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }

  delay(30);
}
