const int PULSE_PIN = A0;

const float ALPHA_DC = 0.95;
const float ALPHA_AC = 0.3;

const int HYST = 3;
const unsigned long MIN_INTERVAL = 300;

float dcLevel = 0;
float filtered = 0;
bool dcInit = false;

bool above = false;
unsigned long lastBeat = 0;
int bpm = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("raw,filtered");
}

void loop() {
  int raw = analogRead(PULSE_PIN);

  if (!dcInit) {
    dcLevel = raw;
    dcInit = true;
  }

  dcLevel = ALPHA_DC * dcLevel + (1 - ALPHA_DC) * raw;

  float ac = raw - dcLevel;

  filtered = ALPHA_AC * ac + (1 - ALPHA_AC) * filtered;

  unsigned long now = millis();

  if (!above && filtered > HYST) {
    if (now - lastBeat > MIN_INTERVAL) {
      if (lastBeat > 0) {
        unsigned long interval = now - lastBeat;
        bpm = 60000 / interval;
      }
      lastBeat = now;
    }
    above = true;
  } else if (above && filtered < -HYST) {
    above = false;
  }

  Serial.print(raw);
  Serial.print(",");
  Serial.println(filtered);

  delay(10);
}