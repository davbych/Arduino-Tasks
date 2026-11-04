const int PIN_AO = A0;
const int PIN_DO = 2;

const int HYST = 30;
int threshold = 512;
bool state = false;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_DO, INPUT);
}

void loop() {
  int raw = analogRead(PIN_AO);
  int d0 = digitalRead(PIN_DO);

  if (!state && raw < threshold - HYST) state = true;
  if (state && raw > threshold + HYST) state = false;

  Serial.print("AO=");
  Serial.print(raw);
  Serial.print(" DO=");
  Serial.print(d0);
  Serial.print(" SOFT=");
  Serial.print(state);
  Serial.print(" TH=");
  Serial.println(threshold);

  delay(100);
}