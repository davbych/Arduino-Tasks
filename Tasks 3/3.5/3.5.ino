#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int PIN_YELLOW = 10;
const int PIN_GREEN = 11;
const int PIN_RED = 12;

const char CODE[5] = "1234";

char entered[5];
byte enteredCount = 0;
byte wrongCount = 0;
bool locked = false;

void setup() {
  pinMode(PIN_YELLOW, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_RED, OUTPUT);
}

void flashYellow() {
  digitalWrite(PIN_YELLOW, HIGH);
  delay(150);
  digitalWrite(PIN_YELLOW, LOW);
}

void showCorrect() {
  digitalWrite(PIN_GREEN, HIGH);
  delay(3000);
  digitalWrite(PIN_GREEN, LOW);
}

void showWrong() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(PIN_RED, HIGH);
    delay(200);
    digitalWrite(PIN_RED, LOW);
    delay(200);
  }
}

void resetEntry() {
  enteredCount = 0;
  entered[0] = '\0';
}

void checkCode() {
  bool ok = true;
  for (int i = 0; i < 4; i++) {
    if (entered[i] != CODE[i]) {
      ok = false;
      break;
    }
  }
  if (ok) {
    wrongCount = 0;
    showCorrect();
  } else {
    wrongCount++;
    showWrong();
    if (wrongCount >= 3) {
      locked = true;
      delay(10000);
      locked = false;
      wrongCount = 0;
    }
  }
  resetEntry();
}

void loop() {
  if (locked) return;

  char key = keypad.getKey();
  if (!key) return;

  if (key == '*') {
    resetEntry();
    return;
  }

  if (key == '#') {
    if (enteredCount == 4) checkCode();
    return;
  }

  if (key >= '0' && key <= '9') {
    if (enteredCount < 4) {
      entered[enteredCount] = key;
      enteredCount++;
      flashYellow();
      if (enteredCount == 4) checkCode();
    }
  }
}