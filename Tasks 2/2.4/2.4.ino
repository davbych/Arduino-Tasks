int red = 10;
int yellow = 9;
int green = 8;
int time1 = 1000;
int time2 = 2000;
int time5 = 5000;

void setup() {
  Serial.begin(9600);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
}

void loop() {
  Serial.print("t="); Serial.print(millis()); Serial.println(" ms phase=GREEN");
  digitalWrite(green, HIGH); //загорелся зелёный (5 сек)
  delay(time5);              
  digitalWrite(green, LOW); 
  
  for (int i=0; i<3; i++) { //зелёный мигает 3 раза
    Serial.print("t="); Serial.print(millis()); Serial.println(" ms phase=GREEN_BLINK");
    digitalWrite(green, HIGH); delay(time1);
    digitalWrite(green, LOW); delay(time1);
  }

  Serial.print("t="); Serial.print(millis()); Serial.println(" ms phase=YELLOW");
  digitalWrite(yellow, HIGH); //загорелся жёлтый (2 сек)
  delay(time2);
  digitalWrite(yellow, LOW);
  
  Serial.print("t="); Serial.print(millis()); Serial.println(" ms phase=RED");
  digitalWrite(red, HIGH); //загорелся красный (5 сек)
  delay(time5);

  Serial.print("t="); Serial.print(millis()); Serial.println(" ms phase=RED_YELLOW");
  digitalWrite(yellow, HIGH); //загорелся жёлтый (2 сек) + красный
  delay(time2);
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
}
