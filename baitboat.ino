
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
SoftwareSerial gpsSerial(8,9);
TinyGPSPlus gps;
float lattitude,longitude;


#include <Servo.h>

//joystick
#define joyX A0
#define joyY A1
int xValue,yValue;

// I2C Library
#include <Wire.h>
// BH1750 Light Sensor Library
#include <BH1750.h>
// QMC5883L Compass Library
#include <QMC5883LCompass.h>

BH1750 lightMeter;
QMC5883LCompass compass;


Servo servo_1; // servo controller (multiple can exist)
int servo_pin = 3; // PWM pin for servo control
int pos = 0;    // servo starting position

bool poklop = false;
bool svetlo = false;
bool tlacidlo = false;

void setup() {
  // Initialize the serial port.
  Serial.begin(9600);
  
    // Initialize I2C.
   Wire.begin();

   
  if (lightMeter.begin()) {
    Serial.println(F("BH1750 initialised"));
  }
  else {
    Serial.println(F("Error initialising BH1750"));
  }
  
  Serial.println(F("BH1750 Test"));


  // Initialize the Compass.
  compass.init();

  servo_1.attach(servo_pin); // start servo control

  
}

void loop() {
  
   
  while (gpsSerial.available())
  {
    Serial.print ("lattitude: ");
    int data = gpsSerial.read();
    if (gps.encode(data))
    {
      lattitude = (gps.location.lat());
      longitude = (gps.location.lng());
      Serial.print ("lattitude: ");
      Serial.println (lattitude);
      Serial.print ("longitude: ");
      Serial.println (longitude);
    }
  }

  //stlacenie tlacidla na otvorenie poklopu
  if(poklop == false && tlacidlo == false){
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        servo_1.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15); // delay to allow the servo to reach the desired position
      }
    if (pos==90){
        delay(3000); // wait 5 seconds once positioned at 90 degrees
      }
    tlacidlo = true;
  }
  
  if(poklop == false && tlacidlo == true){
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      servo_1.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);
    }
    if (pos==90){
        delay(3000);
      }
    tlacidlo = false;
  }

  
    
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

if(lux<=400 && svetlo == false){
  //spustí sa svetlo/LED
  svetlo = true;
}
else if(lux>400 && svetlo == true){
  //vypnutie svetla
  svetlo = false;
}


  int x, y, z;
  // Read compass values
  compass.read();

  x = compass.getX();
  y = compass.getY();
  z = compass.getZ();

  Serial.print("X: ");
  Serial.print(x);
  Serial.print("   Y: ");
  Serial.print(y);
  Serial.print("   Z: ");
  Serial.println(z);

 
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);
 
  //print the values with to plot or view
  Serial.print(xValue);
  Serial.print("\t");
  Serial.println(yValue);
  if(xValue == 0){
    Serial.print(" IDEM DOPREDU");
    
    }
    else if(xValue > 1020){
      Serial.print(" IDEM DOZADU");
      
    }
  if(yValue == 0){
      Serial.print(" IDEM DOPRAVA");
      
      }
      else if(yValue > 1020){
        Serial.print(" IDEM DOLAVA");
        
      }

  

delay(300);
}
