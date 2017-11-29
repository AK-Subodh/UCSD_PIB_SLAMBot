// Motor driver shield library
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);

void setupMotors(){
  //Set up motor driving
  AFMS.begin();  // create with the default frequency 1.6KHz

  //Set motor speeds.
  leftMotor->setSpeed(50);
  rightMotor->setSpeed(50);
}

void moveForward(){
  //Set motor speeds.
  leftMotor->setSpeed(50);
  rightMotor->setSpeed(50);
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
}

void moveBackward(){
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
}

void motorStop(){
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
}

//Rotates left using both wheels.
void rotateLeftBoth(int turningTime){
  //Set motor speeds.
  leftMotor->setSpeed(50);
  rightMotor->setSpeed(50);
  leftMotor->run(BACKWARD);
  rightMotor->run(FORWARD);
  
  delay(turningTime);

  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
}

//Rotates right using both wheels.
void rotateRightBoth(int turningTime){
  //Set motor speeds.
  leftMotor->setSpeed(50);
  rightMotor->setSpeed(50);
  leftMotor->run(FORWARD);
  rightMotor->run(BACKWARD);
  
  delay(turningTime);

  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
}

//Defining and left and right ultrasonic pins
#define LEFT_BOTTOM_ECHO_PIN 30
#define LEFT_BOTTOM_TRIG_PIN 31

#define RIGHT_BOTTOM_ECHO_PIN 33
#define RIGHT_BOTTOM_TRIG_PIN 32

//Maximum amount of times that the car will try to turn before it becomes a failed turn.
#define MAX_TURNING_ITERATIONS 100

//Tolerance of angle for turning.
#define ANGLE_TOLERANCE (PI/2/100)

//Amount of milliseconds to initially turn.
#define INITIAL_TURNING_TIME 50

//Amount of milliseconds to decrement turning time.
#define TURNING_TIME_CHANGE 1

//Flag to check if bot is turning.
//Need to update in turn function.
bool turning = false;

//Turns to desired angle in radians using both wheels.
bool turnBoth(double targetAngle){

  turning = true;

  double minAngleBound = targetAngle - ANGLE_TOLERANCE;
  double maxAngleBound = targetAngle + ANGLE_TOLERANCE;

  int turningTime = INITIAL_TURNING_TIME;

  bool turningLeft = false;
  
  for(int i = 0; i < MAX_TURNING_ITERATIONS; i++){
    Serial.println("Turning");
    double currentAngle = gyroRead();

    //Check if car is at target angle within tolerance.
    if( (currentAngle > minAngleBound) && (currentAngle < maxAngleBound) ){
      turning = false;
      return true;
    }
    //If the angle is too low rotate left.
    else if(currentAngle <= minAngleBound){
      if( !turningLeft ){
        turningTime -= TURNING_TIME_CHANGE;
        turningLeft = true;
      }
      rotateLeftBoth(turningTime);
    }
    //If the angle is too high rotate right.
    else if(currentAngle >= maxAngleBound){
      if( turningLeft ){
        turningTime -= TURNING_TIME_CHANGE;
        turningLeft = false;
      }
      rotateRightBoth(turningTime);
    }else{
      Serial1.println("Error in turning left.");
    }

    //Read left and right distances from ultrasonic sensors.
    double leftBottomDistance = readUltraSonic(LEFT_BOTTOM_TRIG_PIN, LEFT_BOTTOM_ECHO_PIN);
    double rightBottomDistance = readUltraSonic(RIGHT_BOTTOM_TRIG_PIN, RIGHT_BOTTOM_ECHO_PIN);

    //Detects if there is a left or right edge.
    bool isLeftEdge = detectsEdge(leftBottomDistance);
    bool isRightEdge = detectsEdge(rightBottomDistance);
    
    //If there is a left or right edge return false, indicating that turn failed.
    if(isLeftEdge && turningLeft){
      Serial1.println("Left rotation fail");
      Serial.println("Left rotation fail");
      turning = false;
      return false;
    }

    if(isRightEdge && !turningLeft){
      Serial1.println("Right rotation fail");
      Serial.println("Right rotation fail");
      turning = false;
      return false;
    }
  }

  Serial1.println("Max turning iterations fail.");
  Serial.println("Max turning iterations fail.");
  turning = false;
  
  //If maximum iterations has been reached and angle is still not within tolerance.
  return false;
}

bool turnLeftBoth(){
  double currentAngle = gyroRead();
  double targetAngle = currentAngle + PI/2;

  bool success = turnBoth(targetAngle);
  if(!success){
    //If turn failed, turn back to original angle.
    turnBoth(currentAngle);
  }

  return success;
}

bool turnRightBoth(){
  double currentAngle = gyroRead();
  double targetAngle = currentAngle - PI/2;

  bool success = turnBoth(targetAngle);
  if(!success){
    //If turn failed, turn back to original angle.
    turnBoth(currentAngle);
  }

  return success;
}



#include <Adafruit_LSM9DS0.h>

// Create Adafruit_LSM9DS0 object
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(1000);  // Use I2C, ID #1000

//Global variables for gyroscope data.
double anglez = 0;
double dt = 0;
double prev_time = 0;
double biasz = 0;

//Configuring the gyroscope for a specific range
void configureIMU(void) {
    /* Initialise the sensor */
  if(!lsm.begin()) {
      while(1);
  }
  
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);

  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);

  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);

  for(int i = 0; i < 100; i++) {
    sensors_event_t accel, mag, gyro, temp;
    lsm.getEvent(&accel, &mag, &gyro, &temp);
    biasz += gyro.gyro.z;
  }

  biasz = biasz / 100.;
}

//Reads gyroscope and returns current angle in radians in reference to initial angle to be 0 radians.
double gyroRead() {
    sensors_event_t accel, mag, gyro, temp;

    lsm.getEvent(&accel, &mag, &gyro, &temp);

    dt = (millis() - prev_time) / (1000.0);
    anglez = (anglez) + (gyro.gyro.z - biasz) * (dt);
    
    prev_time = millis();

    return (int(anglez)) * (PI / 180);

}

// Define wheel dimensions in terms of meters. eg. 0.0325 m or 3.25 cm.
#define WHEELRADIUS 0.0325                //3.25 cm
#define CIRCUMFERENCE (2*PI*WHEELRADIUS)    //20.42 cm
#define TICK (CIRCUMFERENCE/8)              //2.55 cm

//Digital pins that can use interrupts.
//Wheel encoder pins.
#define LEFT_ENCODER_PIN 3 // Interrupt pin for the left wheel encoder
#define RIGHT_ENCODER_PIN 2 // Interrupt pin for the right wheel encoder


//Position data for left encoder.
int leftWheelTicks = 0;

//Position data for right encoder.
int rightWheelTicks = 0;

//Setup encoders.
void setupEncoders(){
  //Set left and right encoder pins to be pull resistors.
  //As reccomended in encoder datasheet.
  pinMode(LEFT_ENCODER_PIN, INPUT);
  pinMode(RIGHT_ENCODER_PIN, INPUT);
  digitalWrite(LEFT_ENCODER_PIN, HIGH);
  digitalWrite(RIGHT_ENCODER_PIN, HIGH);
  
  //Convert digital pin numbers to interrupt pin numbers.
  int encoderLeftInterrupt = digitalPinToInterrupt(LEFT_ENCODER_PIN);   //Interrupt pin 3.
  int encoderRightInterrupt = digitalPinToInterrupt(RIGHT_ENCODER_PIN);  //Interrupt pin 2.
  
  //Attach interrupts to left and right encoders that listen for a change on the input.
  //ie. When input changes from 0 to 1 or 1 to 0, it will run the code attached, interrupting any
  //current process.
  attachInterrupt(encoderLeftInterrupt, encoderLeftDist, CHANGE);
  attachInterrupt(encoderRightInterrupt, encoderRightDist, CHANGE);
}

//Interrupt service routine for right encoder
void encoderRightDist(){
  //If we are turning do nothing.
  if(turning == false){
    //Add one tick to wheel distance.
    rightWheelTicks++;
  }
}


//Interrupt service routine for left encoder.
void encoderLeftDist(){
  //If we are turning do nothing.
  if(turning == false){
    //Add one tick to wheel distance.
    leftWheelTicks++;

  }
}

//Based on speed of sound this is constant to convert data from ultrasonics to meters.
#define SPEED_OF_SOUND (29.1*2*100) //2910 meters/ (10^-5 seconds) speed of sound twice due to
                         //distance there AND back.

//Defining pins for downfacing ultrasonic sensors
#define FORWARD_BOTTOM_ECHO_PIN 39
#define FORWARD_BOTTOM_TRIG_PIN 38

#define LEFT_BOTTOM_ECHO_PIN 30
#define LEFT_BOTTOM_TRIG_PIN 31

#define RIGHT_BOTTOM_ECHO_PIN 33
#define RIGHT_BOTTOM_TRIG_PIN 32

//Defining pins for front facing ultrasonic sensors.
//Used for detecting objects.
#define FORWARD_FRONT_ECHO_PIN 41
#define FORWARD_FRONT_TRIG_PIN 40

void setupUltraSonics(){
  // Setting up the input and output for the three ultrasonic sensors
  pinMode(FORWARD_BOTTOM_ECHO_PIN, INPUT);
  pinMode(FORWARD_BOTTOM_TRIG_PIN, OUTPUT);

  pinMode(LEFT_BOTTOM_ECHO_PIN, INPUT);
  pinMode(LEFT_BOTTOM_TRIG_PIN, OUTPUT);

  pinMode(RIGHT_BOTTOM_ECHO_PIN, INPUT);
  pinMode(RIGHT_BOTTOM_TRIG_PIN, OUTPUT);

  pinMode(FORWARD_FRONT_ECHO_PIN, INPUT);
  pinMode(FORWARD_FRONT_TRIG_PIN, OUTPUT);

}

//Returns distance read by ultrasonic sensor in meters.
double readUltraSonic(int TRIG_PIN, int ECHO_PIN){
  digitalWrite(TRIG_PIN, HIGH);
  digitalWrite(TRIG_PIN, LOW);

  //Gets amount of time for sound wave to bounce back.
  //Measured in 10^-5 seconds.
  double duration = pulseIn(ECHO_PIN, HIGH);
  
  //Calculate the distance (in cm) based on the speed of sound.
  double distance = duration / SPEED_OF_SOUND ;

  return distance;
}

void setupBluetooth(){
  Serial1.begin(9600);
  #ifndef ESP8266
    while(!Serial1);     // will pause Zero, Leonardo, etc until Serial1 console opens
  #endif
}



void setup(){
  Serial.begin(9600);
  setupMotors();
  configureIMU();
  setupEncoders();
  setupUltraSonics();
  setupBluetooth();

  while( !(Serial1.available() > 0) ){

  }
  String start = Serial1.readString();

  Serial.println("Starting");
}

//Processing data and command functions.
#define TABLE_DISTANCE 0.10
#define OBJECT_DANGER_DISTANCE 0.10

bool STOP = false;

/*
 * Tells if an edge was detected.
 * @param distanceAway Distance that ultrasonic sensor measures. 
 * @return true if there is an edge, false otherwise.
  */
bool detectsEdge(double distanceAway){
  if(distanceAway > TABLE_DISTANCE){
    return true;
  }else{
    return false;
  }
}

/*
 * Tells if an object was detected.
 * @param distanceAway Distance that ultrasonic sensor measures. 
 * @return true if there is an object, false otherwise.
  */
bool detectsObject(double distanceAway){
  if(distanceAway < TABLE_DISTANCE){
    return true;
  }else{
    return false;
  }
}

/*
 * Reads command from Matlab and rotates car according to command.
 * If the command is 'STOP' the car stops and everything is disabled.
 */
void readCommand(){
  while( !(Serial1.available() > 0) ){
  }
  String command = Serial1.readString();
  command.replace('\n', '\0');
  Serial.print("...");
  Serial.print(command);
  Serial.print("...");
  Serial.println("|");

  if(command == "LEFT"){
    Serial.println("Turning left");
    bool success = turnLeftBoth();
    Serial1.print("CONFIRMATION");
    Serial1.print(",");
    Serial.println(success);
    if(success){
      Serial.print("SUCCESS");
      Serial1.println("SUCCESS");
    }else{
      Serial.print("FAIL");
      Serial1.println("FAIL");
      readCommand();
    }
  }else if(command == "RIGHT"){
    bool success = turnRightBoth();
    Serial1.print("CONFIRMATION");
    Serial1.print(",");
    if(success){
      Serial1.println("SUCCESS");
    }else{
      Serial1.println("FAIL");
      readCommand();
    }
  }else if(command == "REVERSE"){
    moveBackward();
    delay(1000);
  }
  else if(command == "STOP"){
    motorStop();
    STOP = true;
    Serial1.println("STOP IGNORE THISm");
  }else{
    STOP = true;
    motorStop();
    Serial1.println("Error in commands.");
  }
}

void loop(){
  if(STOP){
    return;
  }
  //Get current the current angle that car is facing.
  double currentAngle = gyroRead();

  //Get distance that ultrasonics are reading.
  double forwardBottomDistance = readUltraSonic(FORWARD_BOTTOM_TRIG_PIN, FORWARD_BOTTOM_ECHO_PIN);
  double leftBottomDistance = readUltraSonic(LEFT_BOTTOM_TRIG_PIN, LEFT_BOTTOM_ECHO_PIN);
  double rightBottomDistance = readUltraSonic(RIGHT_BOTTOM_TRIG_PIN, RIGHT_BOTTOM_ECHO_PIN);
  double forwardFrontDistance = readUltraSonic(FORWARD_FRONT_TRIG_PIN, FORWARD_FRONT_ECHO_PIN);

  //Get number of ticks that wheels have traveled since last turn.
  int leftWheelTicksAgain = leftWheelTicks;
  int rightWheelTicksAgain = rightWheelTicks;

  //Checks for forward edge and forward object.
  bool isForwardEdge = detectsEdge(forwardBottomDistance);
  bool isLeftEdge = detectsEdge(leftBottomDistance);
  bool isRightEdge = detectsEdge(rightBottomDistance);
  bool isForwardObject = detectsObject(forwardFrontDistance);

  //If there is no edge in front AND there is no object in front, keep moving forward.
  if(!isForwardEdge && !isLeftEdge && !isRightEdge && !isForwardObject){
    moveForward();
  }
  //If there is an edge OR object in front of the car, do the following code.
  else{
    motorStop();
    Serial1.print("SPACE");
    Serial1.print(",");
    if(isForwardEdge){
      Serial1.print("FORWARDEDGE");
    }
    else if(isLeftEdge){
      Serial1.print("LEFTEDGE");
    }
    else if(isRightEdge){
      Serial1.print("RIGHTEDGE");
    }
    else if(isForwardObject){
      Serial1.print("OBJECT");
    }
    else{
      Serial1.println("Error in detecting edge or object.");
    }

    //Send wheel ticks to Matlab.
    Serial1.print(",");
    Serial1.print(leftWheelTicks);
    Serial1.print(",");
    Serial1.println(rightWheelTicks);

    //Resets wheel ticks recorded.
    leftWheelTicks = 0;
    rightWheelTicks = 0;

    //Reads a command from matlab and turns in direction received.
    readCommand();

  }
  //Return to beginning of loop again.
}




