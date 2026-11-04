const int LM35 = A0;
const int MODE = 1;

void setup() {
  Serial.begin(9600);
  if (MODE == 2) analogReference(INTERNAL);
  delay(100);
}

float readTemp() {
  long sum = 0;
  for (int i = 0; i < 16; i++) {
    sum += analogRead(LM35);
    delay(5);
  }
  float raw = sum / 16.0;
  float vref = (MODE == 2) ? 1.1 : 5.0;
  float voltage = raw * vref / 1024.0;
  return voltage * 100.0;
}

void loop() {
  float t = readTemp();
  Serial.println(t, 1);
  delay(500);
}
