# fire-detection-system
 The flame sensor used is of the MH Sensor Series which senses infrared radiation for fire detection.
Smoke Detection: It is a MQ-2 smoke sensor that would be used for smoke or gas particle detection in the air.
# Visual Indicators:
Green LED: This shows that the system is working fine and there is no fire or smoke.
Red LED: In case of fire or smoke, this LED will be lit.
# Alerting Mechanism:
Buzzer: It is there to create an audible alert; in case of fire or smoke, it senses
OLED Display: It shows real-time values of fire and smoke sensors
Water Pump Control: The controlling part of the water pump; a relay module integrates this
Blynk IoT Integration: Remote monitoring and control via the Blynk cloud platform, involving remote alerts and the activation of the water pump.
# Development Overview:
# System Setup:
**Sensors**: Set up was done on MH-Sensor flame and MQ-2 smoke sensors to integrate them into ESP32 to give real-time sensor data about the detection of fire and smoke.
Visual Indicators:
Green LED: The absence of fire or smoke—to indicate normal operation.
Red LED: For activation when fire or smoke is detected.
Alerting Mechanism:
Buzzer: To be activated when fire or smoke is sensed, to alert the user.
OLED Display: Shows system state and sensor reading at that instant.
Relay and Water Pump: The relay module does this; it controls the pump of the water so that when any fire or smoke is detected, it can suppress the fire by then.
# Code Progress:
It reads sensor data from the fire as well as smoke sensors and makes a comparison with preset thresholds.
In case a sensor value crosses the set threshold (for either smoke or fire), the Red LED and Buzzer will be turned on, displaying "Fire Detected!" or "Smoke Detected!" on the OLED.
A 10-second time constant initializes the system reset action if the condition is cleared. The system then returns to its normal operation, with the Green LED on.
# Current Status:
**System Functionality**: System functioning is as expected. The Green LED is on, defaulting to showing real-time readings from the fire and smoke sensors on the OLED display.
**Fire Detection**: Upon detection of fire by the sensor, the Red LED, buzzer, and water pump are initiated, and on the OLED it shows "Fire Detected!"
Smoke Detection**: When it checks smoke, it makes the Red LED plus buzzer and water pump operational, meanwhile displaying "Smoke Detected!" on the OLED.
**Relay and Water Pump**: The relay is used to control the water pump for detecting fire or smoke.
# What Else:
# Blynk IoT Integration Done:
Well, add a Blynk dashboard for ensuring real-time remote monitoring via that. Also, add the provision for controlling the water pump through Blynk—switch it on from any remote location upon fire. Water Pump Integration:
Now, trigger the water’s pumping by that pump through this relay module; the pump will fire as and when the detection of fire or smoke is done.

System Testing: End-to-end testing being performed under actual circumstances by throwing real fire or smoke to check all modules' working in real sense.
Verify notifications and water pump activation come up by running remote control tests through the Blynk platform.
# IoT Integration with Blynk:
**Live Monitoring:** Sensor values of the fire and smoke sensor can be monitored in real time using the Blynk platform.
**Remote Notification:** In case of any fire or smoke detection, the user gets real-time push notification as well as an email, which keeps them updated even if they are far from the system.
**Water Pump Control:** Water pump can be controlled remotely through the Blynk app by switching it on or off based on sensor data.
**User Interface:** The Blynk platform comprises a very interactive front end through which sensor data can be observed, real-time alerts can be received, and the system devices can be controlled through a mobile phone or web browser.
# Blynk Dashboard:
Fire Sensor: Gauge showing the current value of the fire sensor.
Smoke Sensor: Gauge showing the current value of the smoke sensor.
Notifications: Any change or rise in predefined sensor values beyond the specified limit counts for the incidence of fire or smoke. Blynk dashboard for instant update and control of system is accessible at:
Blynk Dashboard
# Conclusion:
The Fire and Smoke Detection System has been made operational with provision of automatic fire and smoke detection along with real-time monitoring and alerting features. Furthermore, integration with Blynk makes the entire system remotely controllable with remote notification. Thus, it is a reliable, cost-effective, automated solution to enhancing fire safety and emergency response in home as well as at work or in an industrial space.
Scaling: Scalability is related to how the application can scale for larger facilities and varying ambient conditions.
Cloud IoT Platform Expansion: Looking for other IoT cloud platforms to enjoy better scalability and services.
