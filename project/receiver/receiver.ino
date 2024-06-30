//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Ultrasonic.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

  //Motor left Forwared
const int motorPin2 =2;
  //Motor right backwared
const int motorPin3 =3;
  //Motor right Forwared
const int motorPin4 =4;
  // Motor left backwared
const int motorPin5 =5;

  //trigPin
const int trigPin =6;
  // echoPin
const int echoPin =7;
 // Communications array holding state of buttons and joystick X- and Y-reading 

const char Ultramsg []    ="StopCar";

const char m = 'm';
const char d ='d';
Ultrasonic ultrasonic(trigPin, echoPin);


//address through which two modules communicate.
const byte address[6] = "00001";

void setup()
{

 
  while (!Serial);
    Serial.begin(9600);
  
   radio.begin();
   pinMode(motorPin2,OUTPUT);
   pinMode(motorPin3,OUTPUT);
   pinMode(motorPin4,OUTPUT);
   pinMode(motorPin5,OUTPUT);
    //set the address
   radio.openReadingPipe(0, address);
   radio.openWritingPipe(address);
   radio.setPALevel(RF24_PA_MAX);
  //Set module as receiver
   radio.startListening();

  
}

void loop()
{

   
    int distanceForwared = ultrasonic.read(CM);

    int metal=analogRead(A0);


    
    if (radio.available()) { // Get remote transmission
      char joystick[3];
      radio.read(&joystick, sizeof(joystick));
      buggycontrol(joystick,metal,distanceForwared); // Control the Buggy...
    }
     
    delay(10);
 // sendbutton = analogRead(SEND_BUTTON);
      radio.stopListening(); 

 
    
    delay(10);
     radio.startListening(); 
     

    
}

void buggycontrol(char* t,int metal,int distanceForwared) { // Control the Buggy
    if(strcmp(t,"F")==0){
      if(metal < 10){
         StopCar();
         radio.write(&m, sizeof(m)); // Send state to other Arduino board 
   
         Serial.print("Metal");
         Serial.println(metal);
      }else if(distanceForwared<15){
         StopCar();
         radio.write(&d, sizeof(d)); 
         // Send state to other Arduino board 
         Serial.println(distanceForwared);
        
      }else{
         Serial.println("FF");
         Forward();
      }
      
    }else if(strcmp(t,"B")==0){
        Serial.println("BB");
         Backward();

      
    }else if(strcmp(t,"L")==0){
      
       Serial.println("LL");
       Left();
    }else if(strcmp(t,"R")==0){
       Serial.println("RR");
       Right();
      
    }else if(strcmp(t,"S")==0){
        Serial.println("SS");
        StopCar();

      
    }
}


void Forward()
{
   digitalWrite(motorPin3, LOW); 
   digitalWrite(motorPin5, LOW); 
   
   digitalWrite(motorPin2, HIGH); 
   digitalWrite(motorPin4, HIGH); 
}
void Backward()
{
   digitalWrite(motorPin2, LOW); 
   digitalWrite(motorPin4, LOW); 
   
   digitalWrite(motorPin3, HIGH); 
   digitalWrite(motorPin5, HIGH); 
  
}
void Left()
{
   digitalWrite(motorPin4, LOW); 
   digitalWrite(motorPin5, HIGH); 
   
   digitalWrite(motorPin3, LOW); 
   digitalWrite(motorPin2, HIGH); 
}
void Right()
{
   digitalWrite(motorPin4, HIGH); 
   digitalWrite(motorPin5, LOW); 
   
   digitalWrite(motorPin3, HIGH); 
   digitalWrite(motorPin2, LOW);  
}
void StopCar()
{
  
  digitalWrite(motorPin3, LOW); 
  digitalWrite(motorPin2, LOW); 

  digitalWrite(motorPin5, LOW); 
  digitalWrite(motorPin4, LOW); 
}
