#include <AccelStepper.h>

// Define stepper motors with updated pins (STEP, DIR)
AccelStepper stepper1(AccelStepper::DRIVER, 2, 5);
AccelStepper stepper2(AccelStepper::DRIVER, 3, 6);
AccelStepper stepper3(AccelStepper::DRIVER, 4, 7);

// Defined positions (24 positions)
long int positions[30][3] = {
  {17000, 0, 25000}, {16000, 0, 0}, {32000, 0, 25000}, {29000, 0, 0},
  {45500, 0, 25000}, {43500, 0, 0}, {46000, 22500, 25000}, {44000, 22500, 0},
  {32500, 22500, 25000}, {30500, 22500, 0}, {18000, 22500, 25000}, {16000, 22500, 0},
  // B MOTOR
  {16000, 0, 25000}, {18000, 0, 0}, {29000, 0, 25000}, {32000, 0, 0},
  {43500, 0, 25000}, {45500, 0, 0}, {44500, 22220, 25000}, {46000, 22500, 0},
  {31000, 22500, 25000}, {32500, 22500, 0}, {16500, 22500, 25000}, {18000, 22500, 0}
};

// Home position
long int homePosition[3] = {0, 0, 0};

// Motor limits: {min, max} for each motor
const long motorLimits[3][2] = {
  {-47000, 47000},  // stepper1 limits
  {-22500, 22500},  // stepper2 limits
  {-25000, 25000}   // stepper3 limits
};

// Emergency stop flag
bool emergencyStop = false;

// Variables to store the current position when emergency stop is triggered
long int stoppedPosition[3] = {0, 0, 0};

void setup() {
  Serial.begin(9600);
  
  stepper1.setMaxSpeed(720);
  stepper1.setAcceleration(10000);
  
  stepper2.setMaxSpeed(700);
  stepper2.setAcceleration(10000);
  
  stepper3.setMaxSpeed(800);
  stepper3.setAcceleration(1000);

  Serial.println("Enter a1-a6 to move forward, b1-b6 to move backward,");
  Serial.println("or 0 for Home. For manual movement, use:");

}

bool isPositionValid(long target[3]) {
  for (int i = 0; i < 3; i++) {
    if (target[i] < motorLimits[i][0] || target[i] > motorLimits[i][1]) {
      Serial.print("Position out of bounds");
      Serial.println(i + 1);
      return false;
    }
  }
  return true;
}

void moveToPosition(long int target[3]) {
  if (emergencyStop) {
    Serial.println("Emergency stop active! Reset to continue.");
    return;
  }

  if (!isPositionValid(target)) {
    Serial.println("Invalid position!  :(");
    return;
  }

  Serial.print("Moving to: ");
  Serial.print(target[0]); Serial.print(", ");
  Serial.print(target[1]); Serial.print(", ");
  Serial.println(target[2]);

  // Move motor 1
  stepper1.moveTo(target[0]);
  while (stepper1.distanceToGo() != 0 && !emergencyStop) {
    stepper1.run();
  }
  if (emergencyStop) {
    Serial.println("Movement interrupted by emergency stop!");
    return;
  }

  // Move motor 2
  stepper2.moveTo(target[1]);
  while (stepper2.distanceToGo() != 0 && !emergencyStop) {
    stepper2.run();
  }
  if (emergencyStop) {
    Serial.println("Movement interrupted by emergency stop!");
    return;
  }

  // Move motor 3
  stepper3.moveTo(target[2]);
  while (stepper3.distanceToGo() != 0 && !emergencyStop) {
    stepper3.run();
  }
  if (emergencyStop) {
    Serial.println("Movement interrupted by stop!");
    return;
  }

  Serial.println("Position reached!");
}

void moveToHome() {
  if (emergencyStop) {
    Serial.println("Emergency stop active! Reset to continue.");
    return;
  }

  Serial.println("Returning to Home");

  // Move motor 3
  stepper3.moveTo(homePosition[2]);
  while (stepper3.distanceToGo() != 0 && !emergencyStop) {
    stepper3.run();
  }
  if (emergencyStop) {
    Serial.println("Movement interrupted by stop!");
    return;
  }

  // Move motor 2
  stepper2.moveTo(homePosition[1]);
  while (stepper2.distanceToGo() != 0 && !emergencyStop) {
    stepper2.run();
  }
  if (emergencyStop) {
    Serial.println("Movement interrupted by stop!");
    return;
  }

  // Move motor 1
  stepper1.moveTo(homePosition[0]);
  while (stepper1.distanceToGo() != 0 && !emergencyStop) {
    stepper1.run();
  }
  if (emergencyStop) {
    Serial.println("Movement interrupted by stop!");
    return;
  }

  Serial.println("Home position reached!");
}

void manualMove(char axis, int factor) {
  if (emergencyStop) {
    Serial.println("stop active! Reset to continue.");
    return;
  }

  long newPos;
  int moveAmount = 0;
  
  if (factor == 1) {
    moveAmount = 1000;
  } else if (factor == 5) {
    moveAmount = 5000;
  } else if (factor == -1) {
    moveAmount = -1000;
  } else if (factor == -5) {
    moveAmount = -5000;
  } else if (factor == 10) {
    moveAmount = 10000;
  } else if (factor == -10) {
    moveAmount = -10000;
  } else {
    Serial.println("Invalid manual factor! Use ±1 or ±5 .");
    return;
  }

  if (axis == 'X') {
    newPos = stepper1.currentPosition() + moveAmount;
    if (newPos < motorLimits[0][0] || newPos > motorLimits[0][1]) {
      Serial.println("X-axis move out of limits.");
      return;
    }
    Serial.print("X manual move from ");
    Serial.print(stepper1.currentPosition());
    Serial.print(" to ");
    Serial.println(newPos);
    stepper1.moveTo(newPos);
    while (stepper1.distanceToGo() != 0 && !emergencyStop) {
      stepper1.run();
    }
  } else if (axis == 'Y') {
    newPos = stepper2.currentPosition() + moveAmount;
    if (newPos < motorLimits[1][0] || newPos > motorLimits[1][1]) {
      Serial.println("Y-axis move out of limits.");
      return;
    }
    Serial.print("Y manual move from ");
    Serial.print(stepper2.currentPosition());
    Serial.print(" to ");
    Serial.println(newPos);
    stepper2.moveTo(newPos);
    while (stepper2.distanceToGo() != 0 && !emergencyStop) {
      stepper2.run();
    }
  } else if (axis == 'Z') {
    newPos = stepper3.currentPosition() + moveAmount;
    if (newPos < motorLimits[2][0] || newPos > motorLimits[2][1]) {
      Serial.println("Z-axis move out of limits.");
      return;
    }
    Serial.print("Z manual move from ");
    Serial.print(stepper3.currentPosition());
    Serial.print(" to ");
    Serial.println(newPos);
    stepper3.moveTo(newPos);
    while (stepper3.distanceToGo() != 0 && !emergencyStop) {
      stepper3.run();
    }
  } else {
    Serial.println("Unknown axis for manual move.");
  }
}

// Emergency stop function
void emergencyStopFunction() {
  emergencyStop = true;
  // Store the current position of each motor
  stoppedPosition[0] = stepper1.currentPosition();
  stoppedPosition[1] = stepper2.currentPosition();
  stoppedPosition[2] = stepper3.currentPosition();
  stepper1.stop();  // Stop motor 1
  stepper2.stop();  // Stop motor 2
  stepper3.stop();  // Stop motor 3
  Serial.println("EMERGENCY STOP ACTIVATED!");
}

// Reset function to clear emergency stop state
void resetEmergencyStop() {
  emergencyStop = false;
  // Set the current position of each motor to the stopped position
  stepper1.setCurrentPosition(stoppedPosition[0]);
  stepper2.setCurrentPosition(stoppedPosition[1]);
  stepper3.setCurrentPosition(stoppedPosition[2]);
  Serial.println("Emergency stop reset. System ready.");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    // Emergency stop command
    if (command.equalsIgnoreCase("E") || command.equalsIgnoreCase("STOP")) {
      emergencyStopFunction();
    }
    // Reset emergency stop command
    else if (command.equalsIgnoreCase("R") || command.equalsIgnoreCase("RESET")) {
      resetEmergencyStop();
    }
    // Manual movement commands
    else if (command.startsWith("Xmanual") || command.startsWith("Ymanual") || command.startsWith("Zmanual")) {
      char axis = command.charAt(0);
      String factorStr = command.substring(7);  
      int factor = factorStr.toInt();
      manualMove(axis, factor);
    }
    // Predefined position commands
    else if (command[0] == 'a' || command[0] == 'b') {
      int index = command[1] - '1'; // Convert '1'-'6' to 0-5

      if (index >= 0 && index < 6) {
        if (command[0] == 'a') {
          // a1-a6: Use positions 0-11 (pairs 0,1 to 10,11)
          int pos1 = index * 2;      // First position in pair
          int pos2 = index * 2 + 1;  // Second position in pair
          moveToPosition(positions[pos1]);
          moveToPosition(positions[pos2]);
        } else if (command[0] == 'b') {
          // b1-b6: Use positions 12-23 (pairs 12,13 to 22,23)
          int pos1 = 12 + index * 2;      // First position in pair
          int pos2 = 12 + index * 2 + 1;  // Second position in pair 
          moveToPosition(positions[pos1]);
          moveToPosition(positions[pos2]);

          // Move motor 1 down by 1000 steps before returning home
          long newPos = stepper1.currentPosition() - 1000;
          if (newPos >= motorLimits[0][0] && newPos <= motorLimits[0][1]) {
            Serial.print("Moving motor 1 down by 1000 steps to ");
            Serial.println(newPos);
            stepper1.moveTo(newPos);
            while (stepper1.distanceToGo() != 0 && !emergencyStop) {
              stepper1.run();
            }
            if (emergencyStop) {
              Serial.println("Movement interrupted by stop!");
              return;
            }
          } else {
            Serial.println("Motor 1 move out of limits!");
          }
        }

        // Return to home after completing the movement
        moveToHome();
      } else {
        Serial.println("Invalid command! Use a1-a6 or b1-b6.");
      }
    } 
    else if (command == "0") {
      moveToHome();
    } 
    else {
      Serial.println("Invalid input! Use a1-a6, b1-b6, 0, or manual commands.");
    }

    Serial.println("Enter next command:");
  }
}