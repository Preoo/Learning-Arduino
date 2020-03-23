/*const*/
const int samplerPin = A0;
const int buttonPin = 7;
const long interval_ms = 20;
unsigned long prevMillis = 0;
int samples_count = 50;
volatile bool runFlag = false;
long temp = 0;
int index;

void setup() {
  // put your setup code here, to run once:
  pinMode(samplerPin, INPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  /*Button press raises interrupt*/
  /*buttonPin is HIGH if switch is not pressed*/
  attachInterrupt(digitalPinToInterrupt(buttonPin),keskeytysrutiini,FALLING);
}

void loop() {
  /*Button has been pressed, 
  start counting. Remember to clear flag afterwards.*/
  if(runFlag){
    
    /*Get samples from A0
    I'm using this method instead of storing each reading
    in array as I can skip allocating space for int[samples_count]
    and mess of keeping track of current array index.
    I believe this is fine fro such small sample count.*/
    
    unsigned long currentMillis = millis();
    if(currentMillis - prevMillis >= interval_ms && index < samples_count){
      prevMillis = millis();
      temp += analogRead(samplerPin);
      index++;
      //Serial.println(index);
    }
    if(index >= samples_count && runFlag){
      float avgADC = temp/float(samples_count);
      float LT = -1.198*avgADC + 829.0;
      Serial.print("Avg. reading: ");
      delay(20);
      Serial.println(avgADC);
      delay(20);
      Serial.print("Avg. reading: ");
      delay(20);
      Serial.println(LT, 4);
      index = 0;
      temp = 0;
      runFlag = false;
    }
  }
}

/*Interrupt_function*/
void keskeytysrutiini(){
  runFlag = true;
  Serial.println("Begin sampling.");
}

