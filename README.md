# RASPSP1
Capstone Project - Robot Arm Satellite Positioning Simulator

## Firmware
- Change the stepper motors pinout definition to match your wiring
- Flash the firmware to arduino
- use the software to interface with the arm, or use these commands to control the motors directly
    - G01 X Y Z A B R
        - Move the upper wrist motor to X steps
        - Move the lower wrist motor to Y steps
        - Move the elbow motor to Z steps
        - Move the shoulder motor to A steps
        - Move the base motor to B steps
        - Move the rail motor to R steps
    - SX n
        - Set the maximum speed of the upper wrist motor to n steps/sec
    - SY n
        - Set the maximum speed of the lower wrist motor to n steps/sec
    - SZ n
        - Set the maximum speed of the elbow motor to n steps/sec
    - SA n
        - Set the maximum speed of the base motor to n steps/sec
    - SB n
        - Set the maximum speed of the base motor to n steps/sec
    - SR n
        - Set the maximum speed of the rail motor to n steps/sec
    - Zero
        - Set the current position of all motors to 0

## Software
- Make sure that the URDF model is in the same directory as the python script
- Change the serial port in the python script to match the port that your arduino is connected to
- run the python script, and enter your desired X,Y,Z coordinate
    - Default position is 0 0 0.751
    - X axis : recommend value between 0 to 1
    - Y axis : recommend value between -0.4 to 0.4
    - Z axis : recommend value between 0.2 to 0.751
- the program will calculate the required steps for each joint, and show the result by graphing the simulated position of the robot arm
- click anywhere on the graph to continue, the program will send the command to move the robot arm to the desired position
- enter new coordinates to go to new position
- or return the system to default position before terminating the program

