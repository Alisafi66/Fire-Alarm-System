#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin Definitions
#define RED_LED_PIN 14    //  red LED
#define GREEN_LED_PIN 12  //  green LED 
#define BUZZER_PIN 13     //  buzzer
#define FIRE_SENSOR_PIN 35 //  fire sensor (MH-Sensor-Series)
#define SMOKE_SENSOR_PIN 34 //  smoke sensor (MQ-2)

// OLED Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Fire and Smoke sensor thresholds 
int fireThreshold = 4000;  // Fire sensor threshold 
int smokeThreshold = 500;  // Smoke sensor threshold 

// Parameters for averaging
#define NUM_READINGS 10
int fireReadings[NUM_READINGS];
int smokeReadings[NUM_READINGS];
int fireIndex = 0;
int smokeIndex = 0;
int fireTotal = 0;
int smokeTotal = 0;
int fireAverage = 0;
int smokeAverage = 0;

// Timer for alarm duration
unsigned long alarmStartTime = 0;  // Start time of the alarm
bool alarmActive = false;  // Flag to track if alarm is active

void setup() {
  Serial.begin(115200);  // Start serial communication

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize OLED display (using I2C address 0x3C)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED allocation failed"));
    while (1);  // Stay here if OLED initialization fails
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("System Initialized");
  display.display();
  delay(2000);  // Delay to show initialization message

  // Start with the first Green LED ON 
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Initialize all readings to 0
  for (int i = 0; i < NUM_READINGS; i++) {
    fireReadings[i] = 0;
    smokeReadings[i] = 0;
  }
}

void loop() {
  // Subtract the last reading from the total
  fireTotal = fireTotal - fireReadings[fireIndex];
  smokeTotal = smokeTotal - smokeReadings[smokeIndex];

  // Read the fire and smoke sensor values
  fireReadings[fireIndex] = analogRead(FIRE_SENSOR_PIN);
  smokeReadings[smokeIndex] = analogRead(SMOKE_SENSOR_PIN);

  // Add the new readings to the total
  fireTotal = fireTotal + fireReadings[fireIndex];
  smokeTotal = smokeTotal + smokeReadings[smokeIndex];

  // Move to the next index
  fireIndex = fireIndex + 1;
  smokeIndex = smokeIndex + 1;

  // If we've reached the end of the array, wrap around to the beginning
  if (fireIndex >= NUM_READINGS) {
    fireIndex = 0;
  }
  if (smokeIndex >= NUM_READINGS) {
    smokeIndex = 0;
  }

  // Calculate the average value
  fireAverage = fireTotal / NUM_READINGS;
  smokeAverage = smokeTotal / NUM_READINGS;

  // Print fire and smoke sensor averages to Serial Monitor
  Serial.print("Fire Sensor Average Value: ");
  Serial.println(fireAverage);
  Serial.print("Smoke Sensor Average Value: ");
  Serial.println(smokeAverage);

  // Display on OLED
  display.clearDisplay();
  display.setCursor(0, 0);

  // Check if either the Fire or Smoke Sensor value exceeds the threshold
  if (fireAverage < fireThreshold || smokeAverage > smokeThreshold) {
    // If fire or smoke detected, turn on Red LED and Buzzer
    if (!alarmActive) {
      alarmStartTime = millis();  // Start the alarm timer
      alarmActive = true;  // Set alarm as active
      Serial.println("Alarm triggered! Fire or Smoke detected.");

      // Turn off the first Green LED, and turn on Red LED and Buzzer for 10 seconds
      digitalWrite(GREEN_LED_PIN, LOW);    // Green LED OFF
      digitalWrite(RED_LED_PIN, HIGH);     // Red LED ON
      digitalWrite(BUZZER_PIN, HIGH);      // Buzzer ON
    }

    display.print("Fire/Smoke Detected!");
  } else {
    // If no fire or smoke detected, turn off Red LED and Buzzer
    digitalWrite(RED_LED_PIN, LOW);      // Red LED OFF
    digitalWrite(BUZZER_PIN, LOW);       // Buzzer OFF
    display.print("No Fire/Smoke Detected");
  }

  // Display the sensor values on the OLED
  display.setCursor(0, 20);
  display.print("Fire Value: ");
  display.println(fireAverage);
  display.setCursor(0, 40);
  display.print("Smoke Value: ");
  display.println(smokeAverage);

  // Check if the alarm duration has passed 
  if (alarmActive && (millis() - alarmStartTime >= 10000)) {
    alarmActive = false;  // Reset alarm state
    digitalWrite(RED_LED_PIN, LOW);      // Red LED OFF
    digitalWrite(BUZZER_PIN, LOW);       // Buzzer OFF
    digitalWrite(GREEN_LED_PIN, HIGH);   // Green LED ON 
    Serial.println("Alarm ended.");
  }

  display.display();  // Update OLED display
  delay(500);  // Delay for 500ms before next reading
}
