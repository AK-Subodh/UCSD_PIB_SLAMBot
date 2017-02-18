// ULTRA SONIC VARS
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS0.h>

#define trigPin1 13
#define echoPin1 12
#define trigPin2 9
#define echoPin2 8
#define led 13
#define led2 10
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(1000);  // Use I2C, ID #1000
int acceReadCounter = 1;


#define LSM9DS0_XM_CS 10
#define LSM9DS0_GYRO_CS 9
//Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(LSM9DS0_XM_CS, LSM9DS0_GYRO_CS, 1000);

/* Or, use Software SPI:
  G_SDO + XM_SDO -> tied together to the MISO pin!
  then select any pins for the SPI lines, and the two CS pins above
*/

#define LSM9DS0_SCLK 13
#define LSM9DS0_MISO 12
#define LSM9DS0_MOSI 11


// Byte (one character) read in from Virtual Serial COMM (Bluetooth)
byte inByte = 0;

// Acknowledgment variable (while-loop sentinel)
byte ack = 0;

// Setup
void setup() {  
  // Open up Bluetooth Virtual Serial COMM port
  Serial.begin(9600); 

  // SETUP ULTRA SONIC
  #ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
//  Serial.println(F("LSM9DS0 9DOF Sensor Test")); Serial.println("");
  
  /* Initialise the sensor */
  if(!lsm.begin())
  {
    /* There was a problem detecting the LSM9DS0 ... check your connections */
  //  Serial.print(F("Ooops, no LSM9DS0 detected ... Check your wiring or I2C ADDR!"));
    while(1);
  }
//  Serial.println(F("Found LSM9DS0 9DOF"));
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Setup the sensor gain and integration time */
  configureSensor();
  
  /* We're ready to go! */
 // Serial.println("");

  
  
  // Configure digital for OUTPUT
  pinMode(led,OUTPUT);
}

// Main
void loop() {
  while (!ack) {
    ack = getAck();    
    delay(3000);
  }
  byte serialCmd = getSerialCmd();

  doSerialCmd(serialCmd);

  doSerialCmd(getSerialCmd());

  acceRead();

}



// Performs serial command based on user input
void doSerialCmd( byte cmd ) {
  switch( cmd ) {
    // Turn LED HIGH
    case ('h'):
      ledON();
      printNewCmdLn();
      break;
  
    // Turn LED LOW
    case ('l'):
      ledOFF();
      printNewCmdLn();
      break;

    // Get Ultra Sonic Reading
    case ('g'):
      acceReadCounter++;
      printNewCmdLn();
      break;
  
    // Display CMD List
    case ('c'):
      printCmdList();
      printNewCmdLn();
      break;
  }
}

// Prompts User for input serial command
//    Returns serial command
byte getSerialCmd() {
  byte inByte;
  if (Serial.available()) {
    inByte = Serial.read();
    Serial.write(inByte);
    printNewLn();
    return inByte;
  }
}

// Turns LED ON and writes to Serial
void ledON() {
  digitalWrite(led, HIGH);
  Serial.write("    The LED is ON!");
}

// Turns LED OFF and writes to Serial
void ledOFF() {
  digitalWrite(led, LOW);
  Serial.write("    The LED is OFF!");
}

void acceRead() {
  if (acceReadCounter%2)
  {
  //while(1) {
 /* Get a new sensor event */ 
  sensors_event_t accel, mag, gyro, temp;

  lsm.getEvent(&accel, &mag, &gyro, &temp); 
  Serial.print(millis()); Serial.print(" ");
  // print out accelleration data
  Serial.print(""); Serial.print(accel.acceleration.x); Serial.print(" ");
  Serial.print(""); Serial.print(accel.acceleration.y);       Serial.print(" ");
  //Serial.print("  \tZ: "); Serial.print(accel.acceleration.z);     Serial.println("  \tm/s^2");

  // print out magnetometer data
  //Serial.print("Magn. X: "); Serial.print(mag.magnetic.x); Serial.print(" ");
  //Serial.print("  \tY: "); Serial.print(mag.magnetic.y);       Serial.print(" ");
  //Serial.print("  \tZ: "); Serial.print(mag.magnetic.z);     Serial.println("  \tgauss");
  
  // print out gyroscopic data
  Serial.print(""); Serial.print(gyro.gyro.x); Serial.print(" ");
  Serial.print(""); Serial.print(gyro.gyro.y);       Serial.print("");
  //Serial.print("  \tZ: "); Serial.print(gyro.gyro.z);     Serial.println("  \tdps");
  Serial.print("\n");

  // print out temperature data
  //Serial.print("Temp: "); Serial.print(temp.temperature); Serial.println(" *C");

  //Serial.println("**********************\n");

  //delay(0);
  delay(0);
// }
  }
}



// Prints the command list
void printCmdList() {
  Serial.write("    Commands:\r\n");
  Serial.write("      <h>    Turn ON LED\r\n");
  Serial.write("      <l>    Turn OFF LED\r\n");  
  Serial.write("      <g>    Ultra Sonic Reading\r\n"); 
  Serial.write("      <c>    Command List"); 
}



// Prints a new command line cursor
void printNewCmdLn() {
  printNewLn();
  Serial.write(">");
}

// Prints a modified new line
void printNewLn() {
  Serial.write("\r\n");
}

byte getAck() {
Serial.write("\r\nPress <c> for command list\r\n");
    Serial.write(">");
    // having this if statement messes up the next loop. fails the conditional statement
    //if (Serial.available()){
      byte inByte = Serial.read(); 
      //Serial.println("readbyte");
   // }
   
    if ( inByte == 'c' ) {
      Serial.println(" inbyte == C");
      Serial.write(inByte);
      ack = 1;
      printNewLn();
      printCmdList();
      printNewCmdLn();
      return 1;
    }   
    return 0;
}
void displaySensorDetails(void)
{
  sensor_t accel, mag, gyro, temp;
  
  lsm.getSensor(&accel, &mag, &gyro, &temp);
  
  delay(500);
}

void configureSensor(void)
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_6G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_16G);
  
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_12GAUSS);

  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_2000DPS);
}
