//Alexander Gruhl
//April 24, 2021
//Test code for setting up the colour sensor and conveyor belt
//Colour sensor detects colour of object on the conveyor belt, conveyor belt moves object to pickup location for robot manipulator
//Code also used to find object colour readings range

#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X); //colour sensor

uint16_t red_counter = 0, blue_counter = 0, yellow_counter = 0; //holds the amount of times each colour has been detected in a row by the colour sensor (resets after new colour detected)

//conveyor belt motor pins
const uint16_t motor1pin1 = 2;
const uint16_t motor1pin2 = 7;

const uint16_t motor2pin1 = 3;
const uint16_t motor2pin2 = 4;

void setup(void) {
Serial.begin(9600); //set baud rate to 9600

//set pins connected to conveyor belt motors to output signals
pinMode(motor1pin1, OUTPUT);
pinMode(motor1pin2, OUTPUT);
pinMode(motor2pin1, OUTPUT);
pinMode(motor2pin2, OUTPUT);

//see if colour sensor detected
if (tcs.begin()) {
Serial.println("Found sensor");
} else {
Serial.println("No TCS34725 found ... check your connections");
while (1);
}
 
}
 
void loop(void) {
//colour sensor variables
uint16_t r, g, b, c, colorTemp, lux;
bool highest_colour_80;
 
tcs.getRawData(&r, &b, &g, &c);
colorTemp = tcs.calculateColorTemperature(r, b, g);
lux = tcs.calculateLux(r, b, g);

//print readings from colour sensor
Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
Serial.println(" ");

if((r > b && r*0.80 < b) || (b > r && b*0.80 < r)){
  highest_colour_80 = true;
} else{
  highest_colour_80 = false;
}

if(r < 600 && g < 400 && b < 450){
  red_counter = 0;
  blue_counter = 0;
  yellow_counter = 0;
  Serial.println("Nothing");
} else if(r*0.75 > g && r*0.75 > b){ //red > red*0.75 of other colours
  red_counter++;
  blue_counter = 0;
  yellow_counter = 0;
  if(red_counter >= 3){
    Serial.println("Red");
    run_conveyor_belt();
  }
} else if(highest_colour_85){ //r and b 0.85 of each other
  red_counter = 0;
  blue_counter = 0;
  yellow_counter++;
  if(yellow_counter >= 3){
    Serial.println("Yellow");
    run_conveyor_belt();
  }
} else{
  red_counter = 0;
  blue_counter++;
  yellow_counter = 0;;
  if(blue_counter >= 3){
    Serial.println("Blue");
    run_conveyor_belt();
  }
}
stop_conveyor_belt();
}

void run_conveyor_belt(){ //have conveyor belt motors run for 3.45 seconds to put object in pickup location
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  delay(3000);
}

void stop_conveyor_belt(){ //have conveyor belt stop for 100 milliseconds
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
  delay(100);
}
