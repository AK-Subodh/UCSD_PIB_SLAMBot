///// related to turn angle code

        //Tests turning of the car. Input is a theta angle given by a potentiometer, where min = 0 rad, max = 2 PI rad. 
        //Tune by adjusting k_motor and dt_turning and fudge_factor. check L_wheelbase and r_wheel  
        //Duty cycles are defined between -255 and 255
        //Formula is for assuming that turn is completed in one clock cycle. May not be possible, so saturation is included. 
        
        double L_wheelbase = .1; // ? meters;
        double r_wheel = .025; // ? meters;
        double k_motor = 1; // needs lots of testing. in units of [(rad/s) / (duty_cycle)]. duty_cycle should range from -255 to 255.
        double dt_turning = 100; // in milliseconds;
        double fudge_factor = 1;
        double maxspeed = 255;
        double minspeed = 30;
        double speedlimit = 64;
       
        double pot_value; // will take value 0 to 1023
        double pot_pin = A8;
        
        double theta_next, theta_current; // in radians
        double motor_drive_forward; // in PWM (0 to 256) 
        
        double prev_time_turning = 0;
/////

///// related to gyro angle estimates

        #include <SPI.h>
        #include <Wire.h>
        #include <Adafruit_Sensor.h>
        #include <Adafruit_LSM9DS0.h>
        
        double anglez = 0;
        double dt = 0;
        double prev_time = 0;
        int i = 0; 
        double biasz = 0; 
        Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(1000);  // Use I2C, ID #1000
        
        #define LSM9DS0_XM_CS 10
        #define LSM9DS0_GYRO_CS 9
        
        
        #define LSM9DS0_SCLK 13
        #define LSM9DS0_MISO 12
        #define LSM9DS0_MOSI 11
        
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
        
        // 2.) Set the magnetometer sensitivity
        lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);
      
        // 3.) Setup the gyroscope
        lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);
      
      }
/////

///// related to motor driver shield library
      #include <Adafruit_MotorShield.h>
      #include "utility/Adafruit_MS_PWMServoDriver.h"
      
      // Create the motor shield object with the default I2C address
      Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
      // Or, create it with a different I2C address (say for stacking)
      // Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 
      
      // Select which 'port' M1, M2, M3 or M4. In this case, M1
      Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
      // You can also make another motor on port M2
      Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
/////



void turn( double theta_next, double theta_current, double motor_drive_forward );

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max);

void setup(void) 
{
  ///// set up gyro and printing
      #ifndef ESP8266
        /* This code is confusing because to me it seems as though we don't need it
	    We are not using the ESP8266, so why do we need to check if it's defined?
	    -Josh */
        while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
      #endif
        Serial.begin(9600);
      //  Serial.println(F("LSM9DS0 9DOF Sensor Test")); Serial.println("");
        
        /* Initialise the gyro sensor */
        if(!lsm.begin())
        {
          /* There was a problem detecting the LSM9DS0 ... check your connections */
          Serial.print(F("Ooops, no LSM9DS0 detected ... Check your wiring or I2C ADDR!"));
          while(1);
        }
        Serial.println(F("Found LSM9DS0 9DOF"));
        
        /* Display some basic information on this sensor */
        displaySensorDetails();
        
        /* Setup the sensor gain and integration time */
        configureSensor();
        
        /* We're ready to go! */
       //Serial.println("Begin Calibration");
      
      
      for (i=0; i<100; i++) 
      {
        sensors_event_t accel, mag, gyro, temp;
      
        lsm.getEvent(&accel, &mag, &gyro, &temp); 
      
       
        //biasx = gyro.gyro.x + biasx; 
      
     	biasz = gyro.gyro.z + biasz;
      }
      
      biasz = biasz/100.;
      delay(0);

/* 
Comment by Ammar

Ultrasonic Sensor Code is in this part? - We'll trial and error to see where it fits.
Measures Distance from car to the surface. 
Regular Distance measured by testing was around  6 - 15 cm. 
When driving off, the ultrasonic sensor measured huge distance ( ~ 96 cm). That means it's measuring distance to the ground. 

So, the exact moment the ultrasonic sensor measures distance to the ground => An edge has been recognized by the car. 

Once an edge has been recognized by the car => Call The Turn Function to turn away from the edge to avoid falling off.

Within the Turn Function => Make our theta_next = theta_current + (PI/2) // All angles are in radians /// 

This is assuming that we are testing on a square table with 4 corners (all 90 degrees). 

There may be a delay. Perhaps not from the ultrasonic sensor, but probably from the bluetooth.

That way, all turns that avoid edges, will be 90 degrees away from the edge it's about to fall of from.

*/

  ///// set up turning algorithm
      pinMode(pot_pin, INPUT);
      theta_current = 0;
      motor_drive_forward = 0; //0-256
  ///// set up motor driving
      AFMS.begin();  // create with the default frequency 1.6KHz
      //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
      // Set the speed to start, from 0 (off) to 255 (max speed)
}

void loop(void) 
{  
      // update angle estimate
      /* Get a new sensor event */ 
      sensors_event_t accel, mag, gyro, temp;
    
      lsm.getEvent(&accel, &mag, &gyro, &temp); 
      
      dt = (millis() - prev_time)/(1000.0); 
      anglez = (anglez) + (gyro.gyro.z-biasz)*(dt);
    
      prev_time = millis();
      
      theta_current = (int(anglez) % (360)) *(PI/180);
      Serial.print(" current gyro angle: ");
      Serial.print( theta_current );
      Serial.print("\t\t");
      
  ///// do turning algorithm
      pot_value = analogRead(pot_pin);
	
      /* Turns voltage input value modified by potentiometer into
      	 a radian value used by the turn function to fix turn amount
	*/
      theta_next = mapfloat(pot_value,0,1023,0,2*PI); // min = 0 rad. max = 2 PI rad

      
      //if ( millis() - prev_time_turning > dt_turning )
      //{
        turn(theta_next, theta_current, motor_drive_forward);
        delay(10);
      //}
     
      
      Serial.print( theta_next );
      Serial.println(" " );
  delay(0);
}

void turn( double theta_next, double theta_current, double motor_drive_forward )
{
  ///// calculate amount to turn
      double theta_dot, turn_adjust, motor_drive_L, motor_drive_R;
      theta_dot = theta_next - theta_current; //in radians
      
      turn_adjust = fudge_factor * L_wheelbase / 2.0 * theta_dot / (k_motor * r_wheel * (dt_turning/1000)); //in units of duty cycle,between -255 to 255
      motor_drive_L = constrain( motor_drive_forward + turn_adjust, -speedlimit, speedlimit);
      motor_drive_R = constrain( motor_drive_forward - turn_adjust, -speedlimit, speedlimit);
      
      
  ///// send command to motors      
      if (motor_drive_L > 0)
      {
        leftMotor->setSpeed(mapfloat(motor_drive_L,0,maxspeed,minspeed,maxspeed));
        leftMotor->run(FORWARD);
      }
     
      else 
      { 
        leftMotor->setSpeed( - mapfloat(motor_drive_L,0,maxspeed,minspeed,maxspeed));
        leftMotor->run(BACKWARD);     
      }

      if (motor_drive_R < 0)
      {
        rightMotor->setSpeed(mapfloat(motor_drive_R,0,-maxspeed,minspeed,maxspeed));
        rightMotor->run(BACKWARD);
      }
      else 
      { 
        rightMotor->setSpeed(mapfloat(motor_drive_R,0,maxspeed,minspeed,maxspeed));
        rightMotor->run(FORWARD);     
      }
      //delay(10);
      
      //leftMotor->run(RELEASE);  //This is to unhooks the motor from the controller so you won't be able to control the wheels 
      //rightMotor->run(RELEASE);

      Serial.print(" theta_dot: ");
      Serial.print( theta_dot );
      Serial.print("\t\t\t");

      Serial.print(" turn adjust: ");
      Serial.print( turn_adjust );
      Serial.print("\t\t\t");

      Serial.print(" motor drive L: ");
      if (motor_drive_L > 0)
        Serial.print( mapfloat(motor_drive_L,0,maxspeed,minspeed,maxspeed) );
      else
        Serial.print( mapfloat(motor_drive_L,0,-maxspeed,-minspeed,-maxspeed) );
 
      Serial.print("\t\t\t");
      Serial.print(" motor drive R: ");
      if (motor_drive_R > 0)
        Serial.print( mapfloat(motor_drive_R,0,maxspeed,minspeed,maxspeed) );
      else
        Serial.print( mapfloat(motor_drive_R,0,-maxspeed,-minspeed,-maxspeed) );
      Serial.print("\t\t\t");
    prev_time_turning = millis();
}

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
 return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

