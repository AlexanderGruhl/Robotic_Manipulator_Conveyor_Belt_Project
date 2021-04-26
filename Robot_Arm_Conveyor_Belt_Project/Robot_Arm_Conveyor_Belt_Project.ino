//Alexander Gruhl
//April 24, 2021
//Colour sensor detects colour of object on the conveyor belt, conveyor belt moves object to pickup location, and robot manipulator picks up object and puts it in the correct bin for its colour

#include <Servo.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

Servo joint1;  //joint1 servo
Servo joint2;  //joint2 servo
Servo joint3;  //joint3 servo
Servo joint4;  //joint4 servo
Servo clawservo;  //claw servo (for opening and closing claw)

int16_t theta_1 = 90, theta_2 = 90, theta_3 = map(-65, 0, -90, 180, 90), theta_4 = map(25, 0, 90, 90, 180); //theta for joints 1, 2, 3, and 4, mapped from actual angle with respect to link to servo movement placement

uint16_t red_counter = 0, blue_counter = 0, yellow_counter = 0; //holds the amount of times each colour has been detected in a row by the colour sensor (resets after new colour detected)

const uint16_t claw_open = 160, claw_closed = 30; //open and closed servo angle for claw servo, respectivly
const uint16_t joint1_2_initial_position = 90; //joint 1 and 2 initial position

const uint16_t motor1pin1 = 2, motor1pin2 = 7; //conveyor belt motor 1
const uint16_t motor2pin1 = 3, motor2pin2 = 4; //conveyor belt motor 2

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X); //colour sensor

void setup() {
  Serial.begin(9600);
  joint1.attach(5);  //attaches joint1 servo to pin 5
  joint2.attach(6);  //attaches joint2 servo to pin 6
  joint3.attach(9);  //attaches joint3 servo to pin 9
  joint4.attach(10);  //attaches joint4 servo to pin 10
  clawservo.attach(11);  //attaches the claw servo to pin 11

  initial_position(); //sets robot arm to the initial (home) position at startup
  clawservo.write(claw_open); //makes the claw start in an open position
  
  //sets motor pins to output to send commands to motor
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
 
  //checks if colour sensor is attached, if not then waits until it is attached
  if(tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
  while(1);
  }
}

void loop() {
  uint16_t r, g, b, c, colorTemp, lux; //calculates colour composition in front of colour sensor
  bool highest_colour_80; //stores if the amount of the second highest colour is greater than 80% of the highest colour
 
  tcs.getRawData(&r, &b, &g, &c); //gets the value of each colour amount

  initial_position(); //sets robot arm to the initial (home) position for every loop iteration
  
  if((r > b && r*0.80 < b) || (b > r && b*0.80 < r)){ //if 80% of red (highest value colour) is greater than blue (2nd highest colour) than set bool to true (and vice versa), otherwise bool is false
    highest_colour_80 = true;
  } else{
    highest_colour_80 = false;
  }

  if(r < 600 && g < 400 && b < 450){ //each colour value less than specified amount when nothing in front of colour sensor
    red_counter = 0;
    blue_counter = 0;
    yellow_counter = 0;
    Serial.println("Nothing");
    stop_conveyor_belt();
  } else if(r*0.75 > g && r*0.75 > b){ //if 75% of the red value is greater than the green and blue values, object in front of colour sensor is red, red counter increases and other reset
    red_counter++;
    blue_counter = 0;
    yellow_counter = 0;
    if(red_counter >= 2){ //if object is detected to be red twice than have robot pick up conveyor belt
      Serial.println("Red");
      run_conveyor_belt();
      stop_conveyor_belt();
      pickup();
      left_drop();
    }
  } else if(highest_colour_80){ //if highest_colour_80 is true than colour of object is yellow (calculations done in previous if-else statement), yellow counter increases and other reset
    red_counter = 0;
    blue_counter = 0;
    yellow_counter++;
    if(yellow_counter >= 2){ //if object is detected to be yellow twice than have robot pick up conveyor belt
      Serial.println("Yellow");
      run_conveyor_belt();
      stop_conveyor_belt();
      pickup();
      center_drop();
    }
  } else{ //colour is blue if other conditions not met, blue counter increases while other counters reset
    red_counter = 0;
    blue_counter++;
    yellow_counter = 0;;
    if(blue_counter >= 2){ //if object is detected to be blue twice than have robot pick up conveyor belt
      Serial.println("Blue");
      run_conveyor_belt();
      stop_conveyor_belt();
      pickup();
      right_drop();
    }
  }
}

void initial_position(){ //set robot arm to initial (home) position (x = 20.1961, y = 0, z = 36.6791)
  joint2.write(joint1_2_initial_position);
  theta_3 = map(-65, 0, -90, 180, 90);
  theta_4 = map(25, 0, 90, 90, 180);
  joint3.write(theta_3);
  joint4.write(theta_4);
  delay(1000);
  joint1.write(joint1_2_initial_position);
  delay(750);            
}

void pickup(){ //have robot claw go to location where the conveyor belt puts the object once it stops moving (x = 37.2494, y = 0, z = 9.8918)
  theta_2 = 40;
  clawservo.write(claw_open);
  joint1.write(joint1_2_initial_position);
  joint2.write(theta_2);
  joint3.write(theta_3);
  joint4.write(theta_4);
  delay(1000);
  clawservo.write(claw_closed);
  delay(500);
}

void right_drop(){ //have robot got to inital position and then to location above the rightmost bin  (x = 20.8850, y = 20.8850, z = 10.5625)
  initial_position();
  theta_1 = 135;
  theta_2 = 65;
  theta_3 = map(-110, 0, -180, 180, 0);
  theta_4 = map(45, 0, 90, 90, 180);
  joint1.write(theta_1);
  delay(1000);
  joint2.write(theta_2);
  joint3.write(theta_3);
  joint4.write(theta_4);
  delay(1000);
  clawservo.write(claw_open);
  delay(1000);
}

void left_drop(){ //have robot got to inital position and then to location above the rightmost bin  (x = 20.9682, y = 17.5944, z = 7.2284)
  initial_position();
  theta_1 = 50;
  theta_2 = 70;
  theta_3 = map(-80, 0, -180, 180, 0);
  theta_4 = map(-35, 90, -90, 180, 0);
  joint1.write(theta_1);
  delay(1000);
  joint2.write(theta_2);
  joint3.write(theta_3);
  joint4.write(theta_4);
  delay(1000);
  clawservo.write(claw_open);
  delay(1000);
}

void center_drop(){ //have robot go to the inital position and then to the center bin (x = 19.3792, y = 0, z = 12.0952)
  initial_position();
  theta_2 = 100;
  theta_3 = map(-90, 0, -180, 180, 0);
  theta_4 = map(-55, 90, -90, 180, 0);
  joint2.write(theta_2);
  joint3.write(theta_3);
  joint4.write(theta_4);
  delay(1000);
  clawservo.write(claw_open);
  delay(1000);
}

void run_conveyor_belt(){ //have conveyor belt motors run for 3.45 seconds
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  delay(3450);
}

void stop_conveyor_belt(){ //have conveyor belt stop for 100 milliseconds
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
  delay(100);
}
