# Washing Machine Interface

## Description

This project implements an interface for a washing machine using a microprocessor and physical circuit hardware. The interface allows users to select different washing cycles, temperatures, and durations using dials and buttons.

## Hardware Components

- Microprocessor (e.g., STM32)
- LEDs (Red, Green)
- Buttons (Left, Middle, Right)
- Potentiometers (x3 for cycle, temperature, duration)
- Seven-Segment Display
- Serial Monitor
- Speaker

## Functionality

### Main Features

- **Cycle Selection**: Users can choose from different washing cycles (e.g., Permanent Press, Whites, Colors, Cotton).
- **Temperature Selection**: Users can select the washing temperature using a potentiometer.
- **Duration Selection**: Users can set the washing duration in minutes.
- **Start/Stop Functionality**: The washing cycle can be started or stopped using buttons.
- **Error Handling**: The interface provides feedback if a required parameter is not selected.
- **Timer**: A countdown timer displays the remaining time of the washing cycle.

### Operational Flow

1. **Power On**: When powered on, the washing machine enters standby mode.
2. **Cycle Selection**: Users can spin the dials to select the desired cycle, temperature, and duration.
3. **Parameter Selection**: Once all parameters are selected, the user can start the washing cycle.
4. **Cycle Execution**: The washing machine performs the selected cycle while displaying the remaining time.
5. **Pause/Resume**: Users can pause and resume the washing cycle if needed.
6. **Completion**: At the end of the cycle, the machine alerts the user and returns to standby mode.

## Usage

1. **Power On**: Press the leftmost button to power on the washing machine.
2. **Cycle Selection**: Spin the dials to select the desired cycle, temperature, and duration.
3. **Start Cycle**: Press the rightmost button to start the washing cycle.
4. **Pause/Resume**: Press the rightmost button again to pause or resume the cycle.
5. **Power Off**: Press the leftmost button again to power off the washing machine.

## Dependencies

- ARM mbed library
- Music library for sound generation

## How to Compile and Run

1. Compile the source code using an appropriate compiler for the microprocessor platform.
2. Flash the compiled binary onto the microprocessor.
3. Connect the hardware components as per the circuit diagram.
4. Power on the washing machine and interact with the interface as described in the Usage section.

## Contributors

## Contributors

- Rahul Verma [![LinkedIn](https://img.shields.io/badge/LinkedIn-%230077B5.svg?logo=linkedin&logoColor=white)](https://linkedin.com/in/rahul-verma-a288392b6) : Developed the interface logic, implemented hardware interaction, and conducted testing.

## License

This project is licensed under the [MIT License](LICENSE).
