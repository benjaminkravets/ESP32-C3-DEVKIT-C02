// Include the AccelStepper Library
#include <AccelStepper.h>

// define step constant
#define FULLSTEP 4
#define STEP_PER_REVOLUTION 2048 // this value is from datasheet

// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper stepper(FULLSTEP, 3, 2, 1, 0); // ESP32 pin: GPIO19, GPIO18, GPIO17, GPIO16

void setup() {
  //Serial.begin(9600);
  stepper.setMaxSpeed(-100.0);   // set the maximum speed
  stepper.setAcceleration(-50.0); // set acceleration
  stepper.setSpeed(0);         // set initial speed
  stepper.setCurrentPosition(0); // set position
  //stepper.moveTo(STEP_PER_REVOLUTION); // set target position: 64 steps <=> one revolution
}

void loop() {
  // change direction once the motor reaches target position

  stepper.moveTo(4096);
  //stepper.move(-1);

  stepper.run(); // MUST be called in loop() function
  //if (stepper.currentPosition() % 20 == 0){
  //  Serial.print(F("Current Position: "));
  //  Serial.println(stepper.currentPosition());
  //}

}