int red = 10;
int yellow = 9;
int green = 8;
int time1 = 1000;
int time2 = 2000;
int time5 = 5000;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
}

void loop() {
  digitalWrite(green, HIGH); //загорелся зелёный (5 сек)
  delay(time5);              
  digitalWrite(green, LOW); 
  for (int i=0; i<3; i++) { //зелёный мигает 3 раза
    digitalWrite(green, HIGH); delay(time1);
    digitalWrite(green, LOW); delay(time1);
  }
  digitalWrite(yellow, HIGH); //загорелся жёлтый (2 сек)
  delay(time2);
  digitalWrite(yellow, LOW);
  digitalWrite(red, HIGH); //загорелся красный (5 сек)
  delay(time5);
  digitalWrite(yellow, HIGH); //загорелся жёлтый (2 сек) + красный
  delay(time2);
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
}
