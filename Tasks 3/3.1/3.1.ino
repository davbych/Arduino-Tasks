const int CAR_RED    = 10;
const int CAR_YELLOW = 9;
const int CAR_GREEN  = 8;

const int PED_RED   = 7;
const int PED_GREEN = 6;
const int BUTTON = 2;

const int TIME_GREEN_BLINK = 300;
const int TIME_YELLOW      = 2000;
const int TIME_PED_GREEN   = 5000;
const int TIME_DEBOUNCE    = 50;

bool pedestrianCycle = false;

void setup() {
  pinMode(CAR_RED, OUTPUT);
  pinMode(CAR_YELLOW, OUTPUT);
  pinMode(CAR_GREEN, OUTPUT);
  pinMode(PED_RED, OUTPUT);
  pinMode(PED_GREEN, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  Serial.begin(9600);
  setIdle();
}

void setIdle() {
  digitalWrite(CAR_RED, LOW);
  digitalWrite(CAR_YELLOW, LOW);
  digitalWrite(CAR_GREEN, HIGH);
  digitalWrite(PED_RED, HIGH);
  digitalWrite(PED_GREEN, LOW);
  pedestrianCycle = false;
}

void allCarOff() {
  digitalWrite(CAR_RED, LOW);
  digitalWrite(CAR_YELLOW, LOW);
  digitalWrite(CAR_GREEN, LOW);
}

void runPedestrianCycle() {
  pedestrianCycle = true;
  Serial.println("Pedestrian request accepted");

  allCarOff();
  digitalWrite(PED_RED, HIGH);
  for (int i = 0; i < 3; i++) {
    digitalWrite(CAR_GREEN, HIGH); delay(TIME_GREEN_BLINK);
    digitalWrite(CAR_GREEN, LOW);  delay(TIME_GREEN_BLINK);
  }

  digitalWrite(CAR_YELLOW, HIGH);
  delay(TIME_YELLOW);
  digitalWrite(CAR_YELLOW, LOW);

  digitalWrite(CAR_RED, HIGH);
  digitalWrite(PED_RED, LOW);
  digitalWrite(PED_GREEN, HIGH);
  delay(TIME_PED_GREEN);

  for (int i = 0; i < 5; i++) {
    digitalWrite(PED_GREEN, HIGH); delay(TIME_GREEN_BLINK);
    digitalWrite(PED_GREEN, LOW);  delay(TIME_GREEN_BLINK);
  }

  setIdle();
  Serial.println("Back to idle");
}

void loop() {
  if (!pedestrianCycle && digitalRead(BUTTON) == LOW) {
    delay(TIME_DEBOUNCE);
    if (digitalRead(BUTTON) == LOW) {
      runPedestrianCycle();
      while (digitalRead(BUTTON) == LOW) { delay(10); }
    }
  }
}
