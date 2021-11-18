

/*
 Code by RADEARSR www.rsrprojek.epizy.com
 ___________________________________________
 |Pin ARDUINO | Pin HC04-SR | Pin LAMPU LED |
 |            |             |               |
 |            |             |               |
 |            |             |               |
 |            |             |               |
 |            |             |               |
 |            |             |               |
 |            |             |               |

*/

#include <Servo.h>
Servo servo;


// Initialize Outdoor Sensor HC04-SR
const int oEchoPin = 4;
const int oTrigPin = 5;

// Initialize Indoor Sensor HC04-SR
const int iEchoPin = 6;
const int iTrigPin = 7;

// Initialize Buzzer & Led
const int led    = 9;
const int buzzer = 10;

// Variabel for result distance sensor
int outdoorSensor;
int indoorSensor;

int pos;


void setup() {
  servo.attach(8);
  servo.write(5);
  
  pinMode(oTrigPin, OUTPUT);
  pinMode(oEchoPin, INPUT);

  pinMode(iTrigPin, OUTPUT);
  pinMode(iEchoPin, INPUT);

  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  
  int outdoorSensor = distanceCalc(oTrigPin, oEchoPin);

  int indoorSensor  = distanceCalc(iTrigPin, iEchoPin);

  Serial.println(outdoorSensor);

  if( outdoorSensor > 0 && outdoorSensor < 6 ){
    
    Serial.println("== Buka Pintu ==");
    openDoor();
    
    delay(500);
    
    while( indoorSensor >= 10 ){
        Serial.println("mobil sedang masuk");
        
        indoorSensor = distanceCalc(iTrigPin, iEchoPin);
        Serial.println(indoorSensor);

        if( indoorSensor <= 10 ){

          Serial.println("== Tutup Pintu ==");
          closeDoor();
          delay(500);
          Serial.println("Mobil sudah terparkir...");
          break;
          
        }
      
    }
    
  }

  else if( indoorSensor > 0 && indoorSensor < 10 ){

    Serial.println("Mobil berada didalam garasi");

    while( indoorSensor <= 10 ) {

      Serial.println("Mobil masih digarasi");

      Serial.println(indoorSensor);

      indoorSensor = distanceCalc(iTrigPin, iEchoPin);

      if( indoorSensor == 10 ){

        Serial.println("== Buka Pintu ==");
        openDoor();
        delay(500);
        Serial.println("Mobil terdeteksi sedang keluar");
        break;
        
      }
      
    }

    while( outdoorSensor > 5 ){

      Serial.println("Mobil sedang keluar");

      outdoorSensor = distanceCalc(oTrigPin, oEchoPin);

      Serial.println(outdoorSensor);

      if( outdoorSensor > 0 && outdoorSensor <= 6 ){

        Serial.println("== Tutup Pintu ==");

        Serial.println("Mobil sudah keluar garasi");
        delay(2000);
        closeDoor();
        delay(500);
        break;
        
      }
      
    }
    
  }
  
}


void openDoor(){
    digitalWrite(led, HIGH);
    delay(200);
    tone(buzzer, 700);
    delay(200);
    noTone(buzzer);
    delay(200);
    digitalWrite(led, LOW);
    tone(buzzer, 700);
    delay(200);
    noTone(buzzer);
    delay(200);
    
    for (pos = 6; pos <= 90; pos += 1) { 
    servo.write(pos);              
    delay(15);                       
  }
}


void closeDoor(){
    digitalWrite(led, HIGH);
    delay(200);
    tone(buzzer, 700);
    delay(400);
    digitalWrite(led, LOW);
    noTone(buzzer);
    
    for (pos = 90; pos >= 6; pos -= 1) { 
    servo.write(pos);              
    delay(15);                       
  }
}


int distanceCalc(int trig, int echo){
    digitalWrite(trig, LOW); 
    delayMicroseconds(2);
    
    digitalWrite(trig, HIGH); 
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    
    int duration  = pulseIn(echo, HIGH);
    int distance = duration * 0.034 / 2;

    return distance;
    
}
