# ESP32RadioControlledArm
A multi-joint robotic arm prototype controlled by an ESP32 microcontroller and external radio receiver input.
This project focuses on mechanical prototyping, servo control, and debugging real-world motion issues such as joint binding, torque limits, and power stability. The goal is to practice engineering workflow.

## Demo
[![demo video](https://img.youtube.com/vi/GkcTVYBv-Io/hqdefault.jpg)](https://youtu.be/GkcTVYBv-Io?si=8iawj52zFQVG05ga)

[Watch on YouTube](https://youtu.be/GkcTVYBv-Io?si=8iawj52zFQVG05ga)

## Current Status

### Working
- Base rotation joint assembled
- Shoulder joint assembled
- External 5V power distribution implemented
- Serial control and calibration functional
- RC receiver PWM signals detected by microcontroller

### In Progress
- Assemble elbow joint and wrist joints (rotate and pitch)
- Reducing joint sag under load
- Improving mechanical stability

## Hardware

- ESP32-S3
- 1x 25kg, 2x 20kg, 2 mid torque, 1 low torque servos
- External regulated 5V power supply
- Radio receiver (PWM channel output)
- Small breadboard and jumper wires
- 3D printed parts

## Purpose

This project is intended as a hands-on learning platform for embedded systems, mechanical design, and debugging.  
Rather than a finished product, it is an iterative engineering prototype focused on understanding how electrical, mechanical, and software systems interact :D
