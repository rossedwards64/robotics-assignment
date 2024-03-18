# Maze Solver

## Steps
The program works as follows:

1. Waits for user to press the 'A' button.

2. Initialises the line and proximity sensors, and calibrates the line
sensor by turning the robot several times and gathering readings. 
	- See MotorHandler::calibrate for the calibration routine.

3. Waits for the user to press one of the 'A', 'B' or 'C' buttons. 
Each button corresponds to a scenario.

4. Chooses a scenario function based on the button press.
   - Scenarios are selected from an array of pointers to scenario
functions.

5. Displays a message to mark the start of the scenario, then executes
the scenario.

6. Once the scenario has terminated, asks the user to select another
scenario.

## Scenario Behaviour

### Scenario 1
Scenario 1 behaves as follows:
1. Takes a reading from the line sensors.
2. Chooses a direction to move in depending on if and where a line is
found.
3. Moves in that direction.
4. Takes a reading from the proximity sensors.
5. If an object is found anywhere in front of the robot, it will stop,
display a celebratory message, then attempt to go back to the start of
the map.
6. If a delivery was made (an object was found), then the scenario
terminates.
		
### Scenario 2
Scenario 2 behaves as follows:
1. Same as [Scenario 1](#scenario-1), however the scenario terminates
once all five deliveries have been made.

### Scenario 3
Scenario 3 behaves as follows:
1. Currently, nothing.


