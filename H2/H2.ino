/*const*/
const int ledPin = 12;
const int buttonPin = 7;
volatile int buttonVar = 0;
volatile int ledState = LOW;
int temp = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  /*Button press raises interrupt*/
  /*buttonPin is HIGH if switch is not pressed*/
  attachInterrupt(digitalPinToInterrupt(buttonPin),kuvaavanimi,FALLING);
  while(!Serial){}
  Serial.println("Ready");
}

void loop() {
  // put your main code here, to run repeatedly:
  /*buttonVar = digitalRead(buttonPin);
  if(buttonVar != temp){
    ledState = !ledState;
  }
  
  Serial.println(buttonVar);
  temp = buttonVar;
  delay(100);
  */
}

/*Interrupt_function*/
void kuvaavanimi(){
  buttonVar++;
  Serial.print("From interrupt: ");
  Serial.println(buttonVar);
  ledState = !ledState;
  digitalWrite(ledPin,ledState);
}

