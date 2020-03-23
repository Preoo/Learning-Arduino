int ledPin = 12;
int delayMS = 2000;
int onOff = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ledPin, HIGH);
  Serial.println("Led on");
  delay(delayMS);
  digitalWrite(ledPin, LOW);
  Serial.println("Led off");
  delay(delayMS);
}


