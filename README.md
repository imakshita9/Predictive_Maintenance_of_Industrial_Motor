#  Predictive maintenance of Industrial motor
https://github.com/imakshita9/PG_PROJECT/assets/100114587/3da317a1-2958-43a2-acbd-414111ecc1c4

Like any other machines, Motor should undergo maintenance and repairs like lubricating, cleaning, and inspecting the machine for potential problems, so it's important to address any potential concerns before they cause serious issues. Instead of taking reactive measures when the damage has already happened, we can make use of condition monitoring based predictive maintenance that will help to avoid any severe issues and extend the life of a motor

Predictive Maintenance is a condition based maintenance strategy uses the actual condition of the equipment to plan what maintenance needs to be performed and when it should be carried out to prevent unplanned motor failure and unnecessary downtime that can greatly affect overall production cost. Motors, whether used to drive conveyors, pumps, cooling fans, or any other machinery, are best viewed as core parts of “systems.” In this project our aim is to reduce unplanned breakdown time and prevent the occurance of motor failure which save money on reaplcements and minimize the overall production cost.

1 Requirement: -

Hardware Requirement:-

MPU6050 – 3 axis Accelerometer Gyroscope
K-Type Thermocouple with MAX6675 Amplifier
Wi-Fi module ESP32
Raspberry PI
Breadboard
Software Requirement:-

Arduino ide Software
Python
VS code
AWS (Amazon Web Services)
2 Model Description

1)MPU6050 – 3 axis Accelerometer Gyroscope:-

MPU6050 sensor moduleIt combines 3-axis Gyroscope, 3-axis Accelerometer and Digital Motion Processor all in small package. it has
additional feature of on-chip Temperature sensor. It has I2C bus interface to communicate with the microcontrollers. The resulting signal is amplified, demodulated, and filtered to produce a voltage that is proportional to the angular rate. - This voltage is digitized using 16-bit ADC to sample each axis. - The full-scale range of output are +/- 250, +/- 500, +/- 1000, +/- 2000. - It measures the angular velocity along each axis in degree per second unit.

K-Type Thermocouple with MAX6675 Amplifier:-

K-Type Thermocouple with the MAX6675 amplifier. A K-type thermocouple is a type of temperature sensor with a wide measurement range like −200ºC to 1350ºC (−326 to 2300ºF) goes above the threshold level the Buzzer starts beeping indicating Danger.  Direct digital conversion of k-type thermocouple output  Cold-junction compensation  Simple SPI-compatible serial interface  Operating voltage range: 3.0 to 5.5V  Operating temperature range: -20 to 85ºC  Resolves temperatures to 0.25ºC, allows readings as high as 1024ºC (1875ºF).

Wi-Fimodule ESP32:-

A feature-rich MCU with integrated Wi-Fi andBluetooth connectivity for a wide-rangeof applications,Robust Design, Ultra-Low Power Consumption, High Level of Integration, Hybrid Wi-Fi & Bluetooth Chip

Raspberry Pi 3 Model B:-

• Includes Made in UK Raspberry Pi 3 (RPi3) Model B Quad-Core 1.2 GHz 1 GB RAM On-board WiFi and Bluetooth Connectivity

• 16 GB Micro SD Card (Class 10) - Raspberry Pi Recommended Micro SD Card pre-loaded with NOOBS

• 2.0A SMART USB Power Supply with High Quality Micro USB Cable • High Quality Raspberry Pi 3 Case, Premium Quality HDMI Cable

3 WORK FLOW PHASE -

• Setup ESP32 with Sensors: Connect sensors – Temperature and vibration to the ESP32 microcontroller board. ESP32 to read data from these sensors.

• ESP32 to RPi Communication: Establishing communication between the ESP32 and the Raspberry Pi (RPi) using a communication protocol such as MQTT (Message Queuing Telemetry Transport). The ESP32 can publish sensor data to a specific topic or endpoint hosted by the RPi.

• RPi Configuration: RPi act as a gateway and receive data from the ESP32. Configure MQTT broker on RPi

• AWS IoT Setup: Configure AWS IoT Core to establish a secure connection between the RPi and AWS cloud. Create a Thing in the AWS IoT Console to represent the RPi device and generated certificates and keys for authentication.

• DynamoDB Configuration: Set up an AWS DynamoDB table to store the sensors data. Define the table schema with attributes (temperature,vibration) corresponding to the sensor data fields.

• Store Data in DynamoDB: Configure AWS IoT Core to route incoming messages from the RPi to DynamoDB. Define a rule in AWS IoT Core that specifies the DynamoDB table as the target for storing sensor data
