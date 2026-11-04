const int POT = A0;
const int MODE = 1;

int bufAvg[16];
int idxAvg = 0;
long sumAvg = 0;

float ema = 0;

int bufMed[5];
int idxMed = 0;

void setup() {
  Serial.begin(9600);
  int v = analogRead(POT);
  for (int i = 0; i < 16; i++) bufAvg[i] = v;
  for (int i = 0; i < 5; i++) bufMed[i] = v;
  ema = v;
  sumAvg = v * 16L;
}

void loop() {
  int raw = analogRead(POT);
  int filtered;

  if (MODE == 1) {
    sumAvg -= bufAvg[idxAvg];
    bufAvg[idxAvg] = raw;
    sumAvg += raw;
    idxAvg = (idxAvg + 1) % 16;
    filtered = sumAvg / 16;
  } else if (MODE == 2) {
    ema = 0.1 * raw + 0.9 * ema;
    filtered = (int)ema;
  } else {
    bufMed[idxMed] = raw;
    idxMed = (idxMed + 1) % 5;
    int tmp[5];
    for (int i = 0; i < 5; i++) tmp[i] = bufMed[i];
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4 - i; j++)
        if (tmp[j] > tmp[j + 1]) {
          int t = tmp[j];
          tmp[j] = tmp[j + 1];
          tmp[j + 1] = t;
        }
    filtered = tmp[2];
  }

  Serial.print(raw);
  Serial.print(' ');
  Serial.println(filtered);
  delay(10);
}