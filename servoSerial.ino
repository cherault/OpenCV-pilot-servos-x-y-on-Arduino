#include <Servo.h>

Servo servo1; Servo servo2; 

void setup() 
{
 servo1.attach(11); //pin x servo
 servo2.attach(10); //pin y servo

 Serial.begin(19200);
}

void loop() 
{
 static int v = 0;

 if ( Serial.available()) 
 {
   char ch = Serial.read();

   switch(ch) 
   {
     case '0'...'9':
       v = v * 10 + ch - '0'; //identify rules to extract x, y values
       break;
     case 'x': //read x value on serial port
       servo1.write(v);
       v = 0;
       break;
     case 'y': //read y value on serial port
       servo2.write(v);
       v = 0;
       break;
   }
 }
}
