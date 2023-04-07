#include <AccelStepper.h>
#include <Servo.h>
#include <MultiStepper.h>

// Upper Wrist
#define X_STEP_PIN         9
#define X_DIR_PIN          8
#define X_ENABLE_PIN       -1

// Lower Wrist
#define Y_STEP_PIN         6
#define Y_DIR_PIN          5
#define Y_ENABLE_PIN       -1

// Elbow
#define Z_STEP_PIN         11
#define Z_DIR_PIN          10
#define Z_ENABLE_PIN       -1

// shoulder
#define A_STEP_PIN         32 // red
#define A_DIR_PIN          33 // blue
#define A_ENABLE_PIN       -1 // E0_ENABLE_PIN

#define B_STEP_PIN         36 // E1_STEP_PIN
#define B_DIR_PIN          37 // E1_DIR_PIN
#define B_ENABLE_PIN       -1 // E1_ENABLE_PIN

#define R_STEP_PIN         28 // red
#define R_DIR_PIN          29 // blue
#define R_ENABLE_PIN       -1 // E1_ENABLE_PIN

// The number of steppers that we want to manage
// #define MULTISTEPPER_NUM_STEPPERS 5
#define MULTISTEPPER_NUM_STEPPERS 6

// Define the steppers and the pins it will use
AccelStepper StepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper StepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper StepperZ(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);
AccelStepper StepperA(AccelStepper::DRIVER, A_STEP_PIN, A_DIR_PIN);
AccelStepper StepperB(AccelStepper::DRIVER, B_STEP_PIN, B_DIR_PIN);
AccelStepper StepperR(AccelStepper::DRIVER, R_STEP_PIN, R_DIR_PIN);


//Current Motor config:
// [X, Y, Z, A, B]
// [Upper Wrist, Lower Wrist, Elbow, Shoulder, Base]

// Up to 10 steppers can be handled as a group by MultiStepper
MultiStepper steppers;

// String to hold input command
String inString = "";

int speed = 2 * 25;

String getSubstring(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup()
{  
  long positions[MULTISTEPPER_NUM_STEPPERS];

  // Configure stepper X - upper wrist
  StepperX.setMaxSpeed(50);
  StepperX.setSpeed(50);
  StepperX.setEnablePin(X_ENABLE_PIN);
  // // Configure stepper Y - lower wrist
  StepperY.setMaxSpeed(33);
  StepperY.setSpeed(33);
  StepperY.setEnablePin(Y_ENABLE_PIN);
  // // Configure stepper Z
  StepperZ.setMaxSpeed(250);
  StepperZ.setSpeed(250);
  StepperZ.setEnablePin(Z_ENABLE_PIN);
  // Configure stepper A
  StepperA.setMaxSpeed(30);
  StepperA.setSpeed(30);
  StepperA.setEnablePin(A_ENABLE_PIN);
  // // Configure stepper B
  StepperB.setMaxSpeed(50);
  StepperB.setSpeed(50);
  StepperB.setEnablePin(B_ENABLE_PIN);

  // configure stepper R
  StepperR.setMaxSpeed(500);
  StepperR.setSpeed(500);
  StepperR.setEnablePin(B_ENABLE_PIN);

  // Then give them to MultiStepper to manage
  steppers.addStepper(StepperX);
  steppers.addStepper(StepperY);
  steppers.addStepper(StepperZ);
  steppers.addStepper(StepperA);
  steppers.addStepper(StepperB);
  steppers.addStepper(StepperR);


  // Set all the steppers to the initial position
  for (int i = 0; i < MULTISTEPPER_NUM_STEPPERS; i++)
    positions[i] = 0;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();  // Blocks until all are in position

  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }
}

void loop()
{
  long positions[MULTISTEPPER_NUM_STEPPERS];  // Array of stepper positions
  String gcode;
  long steps;
  int i;

  // Read serial input:
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    // If you get a newline or a carry return, parse the string
    if (inChar == '\r' || inChar == '\n') {
      // The first substring is the G-code
      gcode = getSubstring(inString, ' ', 0);
      // G01: Linear interpolation
      if (gcode == "G01") {
        Serial.print("G01");
        for (i = 0; i < MULTISTEPPER_NUM_STEPPERS; i++) {
          String str = getSubstring(inString, ' ', i + 1);
          positions[i] = str.toInt();
          Serial.print(' ');
          Serial.print(positions[i]);
        }
        Serial.println();
        steppers.moveTo(positions);
        steppers.runSpeedToPosition();  // Blocks until all are in position

      // Set StepperX speed
      } else if (gcode == "SX") {
        String str = getSubstring(inString, ' ', 1);
        int speed = str.toInt();
        if (speed >= 0 && speed <= 1000) {
          StepperX.setMaxSpeed(speed);
          Serial.print("S ");
          Serial.println(speed);
        } else {
          Serial.println("SX: Invalid Speed");
        }

      // Set StepperY speed
      } else if (gcode == "SY") {
        String str = getSubstring(inString, ' ', 1);
        int speed = str.toInt();
        if (speed >= 0 && speed <= 1000) {
          StepperY.setMaxSpeed(speed);
          Serial.print("SY ");
          Serial.println(speed);
        } else {
          Serial.println("SY: Invalid Speed");
        }

      // Set StepperZ speed
      } else if (gcode == "SZ") {
        String str = getSubstring(inString, ' ', 1);
        int speed = str.toInt();
        if (speed >= 0 && speed <= 5000) {
          StepperZ.setMaxSpeed(speed);
          Serial.print("SZ ");
          Serial.println(speed);
        } else {
          Serial.println("SZ: Invalid Speed");
        }
      // Set StepperA speed
      } else if (gcode == "SA") {
        String str = getSubstring(inString, ' ', 1);
        int speed = str.toInt();
        if (speed >= 0 && speed <= 1000) {
          StepperA.setMaxSpeed(speed);
          Serial.print("SA ");
          Serial.println(speed);
        } else {
          Serial.println("SA: Invalid Speed");
        }
      // Set StepperB speed
      } else if (gcode == "SB") {
        String str = getSubstring(inString, ' ', 1);
        int speed = str.toInt();
        if (speed >= 0 && speed <= 1000) {
          StepperB.setMaxSpeed(speed);
          Serial.print("SB ");
          Serial.println(speed);
        } else {
          Serial.println("SB: Invalid Speed");
        }
        
      } else if (gcode == "SR") {
        String str = getSubstring(inString, ' ', 1);
        int speed = str.toInt();
        if (speed >= 0 && speed <= 1000) {
          StepperR.setMaxSpeed(speed);
          Serial.print("SR ");
          Serial.println(speed);
        } else {
          Serial.println("SR: Invalid Speed");
        }
        
             
      // Offset all motors
      } else if (gcode == "A") {
        String str = getSubstring(inString, ' ', 1);
        int offset = str.toInt();
        if (offset >= 0 && offset <= 2000) {
          for (i = 0; i < MULTISTEPPER_NUM_STEPPERS; i++) {
            positions[i] += offset;
            Serial.print(i);
            Serial.print(":");
            Serial.println(positions[i]);
          }
          steppers.moveTo(positions);
          steppers.runSpeedToPosition();  // Blocks until all are in position
        } else {
          Serial.println("A: Invalid Offset");
        }

      } else if (gcode == "Zero") {
        for (int i = 0; i < MULTISTEPPER_NUM_STEPPERS; i++) {
          positions[i] = 0;          
        }
        StepperX.setCurrentPosition(0);
        StepperY.setCurrentPosition(0);
        StepperZ.setCurrentPosition(0);
        StepperA.setCurrentPosition(0);
        StepperB.setCurrentPosition(0);
        StepperR.setCurrentPosition(0);

        steppers.moveTo(positions);
        steppers.runSpeedToPosition();  // Blocks until all are in position
        Serial.println("Setting Zero");
      } else {
        Serial.println("Unknown G-code");
      }

      // Clear the string for new input
      inString = "";
    } else {
      // Convert the incoming byte to a char and add it to the string
      inString += (char)inChar;
    }
  }
}
