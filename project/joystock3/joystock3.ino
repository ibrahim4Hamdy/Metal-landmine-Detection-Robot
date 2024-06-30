#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h>


int xpin = A0;
int ypin = A1;
int xValue;
int yValue;
LiquidCrystal_I2C lcd(0x27,16,2);
int buzzarpin = 7;

const char Forward[]  ="F";
const char Backward[] ="B";
const char Left[]      ="L";
const char Right []    ="R";
const char Stop []    ="S";

const byte address[6] = "00001";

//create an RF24 object
RF24 radio(9, 10);  // CE, CSN
void setup() {

  radio.begin();
  Wire.begin();
  
  
pinMode (xpin, INPUT);
pinMode (ypin, INPUT);  
pinMode(buzzarpin,OUTPUT);
 
Serial.begin(9600);


lcd.init();
lcd.backlight();

lcd.setCursor(0,0);
lcd.print("mostafa emam");


}

void loop() {
//set the address
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
//Set module as transmitter
  radio.stopListening();

xValue = analogRead(xpin);
yValue = analogRead(ypin);  
 
  if (xValue > 500) {
    radio.write(&Right, sizeof(Right));
    Serial.println("Right");
    printLCD00("Right");
  }else if (xValue < 200){
   
   radio.write(&Left, sizeof(Left));
    Serial.println("Left");
    printLCD00("Left"); 
    
  }else if (yValue > 500) {
    radio.write(&Forward, sizeof(Forward));
    Serial.println("Forward");
    printLCD00("Forward");
  }else if (yValue < 200) {
    radio.write(&Backward, sizeof(Backward));
    Serial.println("Backward");
    printLCD00("Backward");

  }else{
       
    radio.write(&Stop, sizeof(Stop));
    Serial.println("Stop");
    printLCD00("Stop");

    }
  delay(20);
  //set the address
   radio.openReadingPipe(0, address);
   radio.setPALevel(RF24_PA_MAX);
  //Set module as receiver
   radio.startListening();


   if (radio.available()) {

              char text ='n';
              radio.read(&text, sizeof(text));
              Serial.println(text);

         if(text == 'm'){
               digitalWrite(buzzarpin,HIGH);
               Serial.println("alarm");
               printLCD01("Detected");

          }else{
              digitalWrite(buzzarpin,LOW);
              Serial.println("Scan");
              printLCD01("Scan");

            }
              delay(20);

   }else{
      digitalWrite(buzzarpin,LOW);
      Serial.println("Scan");
      printLCD01("Scan");

    
    }
      delay(20);
}


void printLCD01(String text){
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(text);
}
void printLCD00(String text){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(text);
}
