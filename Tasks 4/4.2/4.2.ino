const int RED = 11, GREEN = 10, BLUE = 9;
const int d = 1;

void setColor(uint8_t r, uint8_t g, uint8_t b) {
    analogWrite(RED, r);
    analogWrite(GREEN, g);
    analogWrite(BLUE, b);
}

void setup() {
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
}

void loop() {
    for (int v = 0; v <= 255; v++) {
      setColor(v, 0, 0); delay(d);
    }
      for (int v = 255; v >= 0; v--) {
        setColor(v, 0, 0); delay(d);
      }
    for (int v = 0; v <= 255; v++) {
      setColor(v, v-0.5, 0); delay(d);
    }
      for (int v = 255; v >= 0; v--) {
        setColor(v, v-0.5, 0); delay(d);
      }
    for (int v = 0; v <= 255; v++) {
      setColor(v, v, 0); delay(d);
    }
      for (int v = 255; v >= 0; v--) {
        setColor(v, v, 0); delay(d);
      }
    for (int v = 0; v <= 255; v++) {
      setColor(0, v, 0); delay(d);
    }
      for (int v = 255; v >= 0; v--) {
        setColor(0, v, 0); delay(d);
      }
    for (int v = 0; v <= 255; v++) {
      setColor(0, v-0.5, v); delay(d);
    }
      for (int v = 255; v >= 0; v--) {
        setColor(0, v-0.5, v); delay(d);
      }
    for (int v = 0; v <= 255; v++) {
      setColor(0, 0, v); delay(d);
    }
      for (int v = 255; v >= 0; v--) {
        setColor(0, 0, v); delay(d);
      }
    for (int v = 0; v <= 255; v++) {
      setColor(v, 0, v); delay(d);
    }
      for (int v = 255; v >= 0; v--) {
        setColor(v, 0, v); delay(d);
      }
}
