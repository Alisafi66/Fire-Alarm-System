# fire-detection-system
The goal of this project is to develop a fire detection system which mandates the use of flame and smoke sensors (MH-Sensor-Series and MQ-2) to detect the presence of fire and trigger an alarm. It also provides real-time notifications on an OLED display and alerting to either the Blynk IoT cloud platform for remote monitoring.

Development Overview:

# System Setup:

The setup is complete, and the MH-Sensor flame sensor and the MQ-2 smoke sensor feed information to the ESP32 Development Board.
Visual notifications are indicated with a Red LED for a fire alarm, while Green LED shows the default condition of the system assuming no fire is detected.
For the alert generation, the buzzer is integrated, which sounds upon detection of fire.
An OLED display is integrated to display the fire detection status and real-time sensor values.

# Code Progress:

The code implements the blinking of red light with a buzzer as soon as the fire sensors are exceeded.
The OLED displays system status and sensor readings.
System works based on the calibrated fire detection threshold of fire sensor.

# Current Status:

The system works as one would expect, with the Green LED normally ON, and the OLED display active with fire readings.
When fire presence is detected by the fire sensor (fire sensor value is greater than the threshold), the Red LED and the Buzzer are turned ON and the OLED shows "Fire Detected!". Then after 10 seconds, the system resets itself, back to Green LED ON.
The other smoke sensor, MQ-2, has been integrated, but its sensitivity further needs calibration before it reliably triggers the alarm.
# Current Issues:
The MQ-2 smoke sensor requires adjustments to both sensitivity and threshold if adequate alarm triggering is to be achieved.
The calibration of the fire sensor is ongoing, as it still outputs a continuous high reading. 
# Next Steps:
Fine-tuning of the smoke sensor for increased sensitivity.
Integration with the IoT cloud: the next big goal for this is interfacing the entire system with a cloud IoT platform such as Blynk, which allows remote monitoring and notifications. This will involve connecting the ESP32 to Wi-Fi and configuring Blynk for various alerts.
Via this project, remote monitoring for fire detection systems will initially let people know about any fire events of which they were aware, even while away from the location it was installed.
Let me know if there are any other modifications you wish to have done or if you would need help with Blynk integration! 
### IoT Integration with Blynk

As part of the project, I have integrated **Blynk IoT platform** for real-time remote monitoring and notifications. You can access the live dashboard for the fire detection system here:

[Blynk Dashboard](https://blynk.cloud/dashboard/681806/global/devices/1/organization/681806/devices/2895555/dashboard)

The dashboard provides real-time updates of the system, and notifications will be sent when fire is detected, enabling remote alerts.
