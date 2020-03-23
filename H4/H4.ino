#include <EEPROM.h>

/*const*/
const int samplerPin = A0;
const int buttonPin = 7;
const int button2Pin = 3;

/*EEPROM variables*/
int eeAddress = 0;
int offset;

struct TempEvent_t {
  unsigned long sec;
  float t_reading;
};

/*Timing variables*/
unsigned long prevMillis = 0ul;
const long interval_ms = 5000l;
unsigned long secs = 0ul;
unsigned long prevLTMillis = 0ul;

/*Temp sampling variables*/
float lastReading;
float LT;
const long sampling_interval_ms = 20l;
int samples_count = 50;
long accumul_temp = 0;
int index;
/*Temperature offset to determine if new value
differs by signifigant amount. Used to avoid pointless
EEPROM writes where temp swings between to close values
such as 22.92 - 22.90. Set to 0.0f to disable offset-check
and fallback to */
float tempOffset = 0.05f;

void setup() {
  // put your setup code here, to run once:
  pinMode(samplerPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(button2Pin, INPUT);
  offset = sizeof(TempEvent_t);
  lastReading = 0.000f;
  Serial.begin(9600);
  /*Button press raises interrupt
  buttonPin is HIGH if switch is not pressed
  detecting change instead of HIGH,LOW to avoid concurrent interrupts
  at the price of phantom presses and false signals caused by noise/shaky fingers.
  Improvements: implement debounce*/
  attachInterrupt(digitalPinToInterrupt(buttonPin),printFromEEPROM,FALLING);
  attachInterrupt(digitalPinToInterrupt(button2Pin),emptyEEPROM,RISING);
  while(!Serial){}
  //Serial.println(EEPROM.length());
  //Serial.println(offset);
  
}

void loop() {
  
  unsigned long currentMillis = millis();
  if(currentMillis - prevMillis >= interval_ms){
    prevMillis = millis();
    /* Debug
    Serial.print("Avg. reading: ");
    Serial.println(LT);
    */
    checkDelta();
  }
  /*V2*/
  if(currentMillis - prevLTMillis >= sampling_interval_ms && index < samples_count){
      prevLTMillis = millis();
      accumul_temp += analogRead(samplerPin);
      index++;
      //Serial.println(index);
    }
    if(index >= samples_count){
      float avgADC = accumul_temp/(float)samples_count;
      LT= -1.198*avgADC + 829.0;
      index = 0;
      accumul_temp = 0;
    }
   /*end V2*/
}

void checkDelta(){
  /*to int or not to int, that is the question.
  perhaps ulong would make more sense here for
  longer operating cycles..
  
  This secs conversion(boxing/casting) still smells..*/
  secs += interval_ms/1000ul;

  /*Not ideal way of doing this... Documentation states,
  that float operations are more expensive on Ard. than int operations.
  */
  if(abs(LT - lastReading) > tempOffset){
    /*Write to eeprom
    increase eeAddress by struct size*/
    TempEvent_t writeEvent = {
        secs,
        LT
    };
    EEPROM.put(eeAddress, writeEvent);
    eeAddress += offset;
    Serial.print("Saved new value of: ");
    Serial.println(LT, 4);
    lastReading = LT;
    /*Check if there is still room for more
    else set EEPROM address to first cell*/
    if(eeAddress + offset >= EEPROM.length()){
      eeAddress = 0;
      Serial.println("EEPROM full, overwriting first entries.");
    }
  }
}



/*Interrupt_functions*/
void printFromEEPROM(){
  
  unsigned int i = 0u;
  TempEvent_t myEvent;
  Serial.println("Event object from EEPROM: time, value");
  while(i < eeAddress){
    
    EEPROM.get(i, myEvent);
    i += offset;
    //Serial.println("Event object from EEPROM: time, value");
    Serial.print(myEvent.sec);
    Serial.print(", ");
    Serial.println(myEvent.t_reading, 4);
    delay(2);
  }
  if(i >= EEPROM.length()){
    i = 0;
  }
  
  Serial.println("End of data");
}
/*Using update instead of write,
to avoid unneeded writes where,
memvalue == newvalue

Objective was to empty EEPROM, but it makes sense
to reset lastReading var to 0.0f, so new data can be 
collected right away. 
*/
void emptyEEPROM(){
  
  for (unsigned int i = 0u ; i < EEPROM.length() ; i++) {
    EEPROM.update(i, 0);
  }
  eeAddress = 0;
  lastReading = 0.000f;
  Serial.println("EEPROM Erased");
}
