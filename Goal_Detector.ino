#include <Servo.h>

const int buttonPinKick = 2; 
const int buttonINC = 3;
const int buttonDEC = 4;
const int redLED = 11;
const int greenLED = 12;
const int pinReceiver = 8; 

int power = 2;  

int buttonStateKick = 0;       
int lastButtonStateKick = 0;   

int buttonStateINC = 0;         
int lastButtonStateINC = 0;     

int buttonStateDEC = 0;
int lastButtonStateDEC = 0;

Servo myservo;  
int pos = 0;    

void setup() {
  Serial.begin(9600);
  pinMode(buttonPinKick, INPUT);
  pinMode(buttonStateINC, INPUT);
  pinMode(buttonStateDEC, INPUT);
  pinMode(pinReceiver, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(pos);
}

void loop() {
  buttonStateKick = digitalRead(buttonPinKick);
  buttonStateINC = digitalRead(buttonINC);
  buttonStateDEC = digitalRead(buttonDEC);
  int dectected = digitalRead(pinReceiver);

  digitalWrite(redLED, LOW); // initializes redLED to LOW
  digitalWrite(greenLED, LOW); // initializes greenLED to LOW

  Serial.println(dectected);




// Increase Power Button 
// Max power = 10
  if (buttonStateINC != lastButtonStateINC) {
    if (buttonStateINC == HIGH) {
      power += 2;

      // if power is more than 10 power is reset to 10
      if (power > 10){
        power = 10;
      }
      Serial.println(power); // prints the value of the power
    } 
    delay(50);
  }
  lastButtonStateINC = buttonStateINC;





// Decrease Power Button
// Min power = 2
  if (buttonStateDEC != lastButtonStateDEC) {
    if (buttonStateDEC == HIGH) {
      power -= 2;
      
      // if power is more than 10 power is reset to 10
      if (power < 2){
        power = 2;
      }
      Serial.println(power); // prints the value of the power
    } 
    delay(50);
  }
  lastButtonStateDEC = buttonStateDEC;



// Kick Button
  if (buttonStateKick != lastButtonStateKick) {
    // If kick button is pressed
    if (buttonStateKick == HIGH) {
      // Move servo motor 180 degrees 
      for (pos = 0; pos <= 180; pos += power) { 
        myservo.write(pos);              
        delay(15);                       
      }
      // Move servo motor back 180 degrees 
      for (pos = 180; pos >= 0; pos -= power) {
        myservo.write(pos);              
        delay(15);                    
      }
      
      int currentTime = 0;
      int finalTime = 100;
      int missFlag = 1; // Miss flag is set to 1

      
      // While loop excutes for 10 seconds after the kick button is pressed
      while (currentTime < finalTime) {
        delay(100);
        int dectected = digitalRead(pinReceiver);
        // When kick button is pressed and the laser is LOW
        if (dectected == HIGH) {
          delay(5000);  // wait 5 seconds 
          // Reset the laser reveiver
          dectected = digitalRead(pinReceiver);
          // If the laser receiver is high after 5 seconds then goal 
          if (dectected == LOW){ 
            missFlag = 0; // Miss flag set to 0
            break;
          }
          // Else the ball is stuck on the laser and no goal
          else{
            missFlag = 1; // Miss flag is set to 1
            break;
          }
        }
        currentTime++; 
      }

      // After while loop is done check missFlag
      // If 1 then No Goal
      // If 0 then Goal 
      if (missFlag == 1){
        Serial.println("no goal");
        digitalWrite(redLED, HIGH); // Red LED on
      }
      else {
        Serial.println("goal");
        digitalWrite(greenLED, HIGH); // Green LED on
      }
      delay(5000);
      Serial.println(power); 
    }
  }
  lastButtonStateKick = buttonStateKick;
}
