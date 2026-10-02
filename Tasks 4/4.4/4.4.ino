const int POT = A0;
const int PIN_R = 9;
const int PIN_G = 10;
const int PIN_B = 11;

// Границы зон (в сырых значениях АЦП 0..1023)
const int ZONE_GREEN_MAX  = 341;    // 0..341   — зелёный
const int ZONE_YELLOW_MAX = 682;    // 341..682 — жёлтый
                                    // 682..1023 — красный

// Ширина плавного перехода между зонами (в единицах АЦП)
const int BLEND = 80;

const unsigned long PRINT_INTERVAL = 200;
unsigned long lastPrint = 0;

void setup() {
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  Serial.begin(9600);
}

int clamp255(int v) {
  if (v < 0) return 0;
  if (v > 255) return 255;
  return v;
}

void loop() {
  int raw = analogRead(POT);

  int r = 0, g = 0, b = 0;

  if (raw < ZONE_GREEN_MAX - BLEND) {
    g = 255;
  } else if (raw < ZONE_GREEN_MAX + BLEND) {
    int t = map(raw, ZONE_GREEN_MAX - BLEND, ZONE_GREEN_MAX + BLEND, 0, 255);
    g = 255;
    r = t;
  } else if (raw < ZONE_YELLOW_MAX - BLEND) {
    r = 255; g = 255;
  } else if (raw < ZONE_YELLOW_MAX + BLEND) {
    int t = map(raw, ZONE_YELLOW_MAX - BLEND, ZONE_YELLOW_MAX + BLEND, 0, 255);
    r = 255;
    g = 255 - t;
  } else {
    r = 255;
  }

  analogWrite(PIN_R, clamp255(r));
  analogWrite(PIN_G, clamp255(g));
  analogWrite(PIN_B, clamp255(b));

  if (millis() - lastPrint >= PRINT_INTERVAL) {
    lastPrint = millis();
    Serial.print("raw=");
    Serial.print(raw);
    Serial.print("  R=");  Serial.print(r);
    Serial.print(" G=");   Serial.print(g);
    Serial.print(" B=");   Serial.println(b);
  }
}