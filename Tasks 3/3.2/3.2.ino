const int LED = 9;
const int BUTTON = 2;

bool ledState = false;

bool rawReading = HIGH;
bool lastReading = HIGH;
bool stableState = HIGH;

unsigned long lastChangeTime = 0;
const unsigned long DEBOUNCE_MS = 50;

unsigned int pressCount = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  rawReading = digitalRead(BUTTON);

  if (rawReading != lastReading) {
    lastChangeTime = millis();
  }

  if (millis() - lastChangeTime > DEBOUNCE_MS) {
    if (rawReading != stableState) {
      stableState = rawReading;

      if (stableState == LOW) {
        ledState = !ledState;
        digitalWrite(LED, ledState);
        pressCount++;
        Serial.print("presses = ");
        Serial.println(pressCount);
      }
    }
  }

  lastReading = rawReading;
}
