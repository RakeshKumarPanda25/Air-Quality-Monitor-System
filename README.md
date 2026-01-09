**PROJECT REPORT: AIR QUALITY MONITOR SYSTEM**

*Abstract*

This project presents a low-cost indoor air quality monitoring and alert system utilising an Arduino Nano, a DHT11 temperature and humidity sensor, an MQ2 smoke/flammable gas sensor, an MQ135 air quality sensor, and a 0.96-inch SSD1306 OLED display. The system estimates concentrations of CO₂, NH₃, benzene, SO₂, and CO from MQ135 and MQ2 readings, displays them with temperature and humidity on the OLED, and flashes an on-screen alert when any gas exceeds predefined safety thresholds.
Introduction and Objective
Poor indoor air quality due to smoke, cooking fumes, volatile organic compounds, and traffic pollution can affect health, concentration, and comfort, especially in small rooms and urban homes. The objective is to monitor temperature, humidity, and key harmful gases continuously, show live values locally, and provide an immediate visual alert when air quality degrades beyond safe limits so that users can ventilate or leave the room.

*Block Diagram*
 
The functional block diagram consists of: Arduino Nano as the central controller; DHT11 providing digital temperature and humidity data; MQ135 and MQ2 providing analog gas concentration signals; SSD1306 OLED receiving processed values over I²C; and a 5 V supply powering the entire system. The Nano reads sensor outputs, computes approximate gas ppm using calibration constants and curve equations, decides whether thresholds are exceeded, and either shows an alert screen or the normal multi-parameter status screen.

*Circuit Diagram:*

In the circuit, the DHT11 data pin connects to digital pin D2 of the Arduino Nano, while its VCC and GND pins connect to 5 V and ground, respectively. The MQ135 analog output is wired to analog pin A0, the MQ2 analog output to analog pin A1, and the SSD1306 OLED is connected via I²C with SDA to A4, SCL to A5, and VCC/GND to 5 V and ground; all sensors share a common ground and 5 V supply.
 
*Components Used and Description*

  Arduino Nano: ATmega328P-based microcontroller board that reads sensors, executes the ppm computation code, and drives the OLED display.
	
  DHT11 sensor: Low-cost digital sensor measuring temperature from 0–50 °C and relative humidity from 20–90%, used for basic indoor comfort monitoring.
	
  MQ135 sensor: Gas sensor sensitive to CO₂, NH₃, benzene, and other harmful gases; its analog resistance change is used to estimate ppm levels of multiple pollutants.
	
  MQ2 sensor: Gas sensor for smoke, LPG, and flammable gases; in this project, it primarily contributes to the CO approximation and general smoke indication.
	
  SSD1306 128×64 OLED: Monochrome I²C display used to show temperature, humidity, and each gas concentration in ppm as well as alert messages.
	
  Supporting parts: Breadboard, jumper wires, and a regulated 5 V USB or adapter supply to power the Nano and sensors.

*Software and Code Explanation:*

The code uses the DHT library to read temperature and humidity, and the Adafruit_GFX and Adafruit_SSD1306 libraries to draw text on the OLED via I²C. Two analog readings (MQ135 on A0 and MQ2 on A1) are converted to sensor voltages, then to sensor resistance Rs, and finally to ppm values using a generic gas-curve model "ppm"=a⋅(Rs/R_0 )^b with specific a and b constants for CO₂, NH₃, benzene, SO₂, and CO.

The main loop performs the following steps every second:

  Read temperature and humidity from the DHT11 and gas ADC values from MQ135 and MQ2.
	
  Compute ppm for CO₂, NH₃, benzene, SO₂ (from MQ135) and CO (from MQ2, using its own constants).
	
  Compare each ppm value to a defined threshold; if any exceed the limit, build an alert string listing the dangerous gases.
	
  If an alert is active, clear the OLED and display “ALERT! High:” plus the gas codes (CO₂, NH₃, C₆H₆, S, CO) for a fixed duration; otherwise, show the normal status layout with temperature, humidity, and each gas value in ppm, and print the same data to the Serial Monitor for debugging.

*Working Principle and System Output*

The working principle is based on the correlation between semiconductor gas sensor resistance and gas concentration: the MQ sensors change their resistance when exposed to target gases, and using a pre-calibrated R_0 and gas curves, the code converts this change into approximate ppm values. In normal operation, the OLED continuously displays temperature, humidity, and ppm values for CO₂, NH₃, benzene, SO₂, and CO; when any value crosses its threshold (e.g., CO₂ above 800 ppm or CO above 40 ppm), the display switches to an alert screen highlighting which gases are high so the user can quickly respond.

*Project Achievements*

  Successfully integrated DHT11, MQ2, MQ135, and SSD1306 OLED with Arduino Nano to create a compact indoor air quality monitor with real-time multi-parameter display.
	
  Implemented threshold-based alert logic and an OLED alert interface that clearly informs the user when air quality becomes hazardous, improving situational awareness compared to raw sensor values alone.

*Key Benefits*

  Low-cost solution using widely available sensors and an Arduino Nano, suitable for students, makers, and low-budget home installations.
	
  Provides a single, portable device that monitors both comfort (temperature and humidity) and multiple harmful gases, enabling early detection of smoke, fumes, or stale, CO2-rich air.

*Conclusion*

This project demonstrates a practical and affordable indoor air quality monitoring and alert system using Arduino Nano, DHT11, MQ2, and MQ135, coupled with an SSD1306 OLED for local visualization. By combining multi-gas sensing with clear visual alerts, the system can help users identify unsafe conditions early and take timely actions such as opening windows, switching on exhaust fans, or evacuating in case of severe smoke or gas leakage.
