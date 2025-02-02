#define BLYNK_TEMPLATE_ID "TMPL4MfJj7V5h"
#define BLYNK_TEMPLATE_NAME "ABDELERIM"
#define BLYNK_AUTH_TOKEN "5OwHNwwMoIsdxVJGShpXeltUMCqG2mTz"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BlynkSimpleEsp32.h>  

// Pin Definitions
#define RED_LED_PIN 14    // Red LED    
#define GREEN_LED_PIN 12  // Green LED
#define BUZZER_PIN 13     // Buzzer
#define FIRE_SENSOR_PIN 35 // Fire sensor (MH-Sensor-Series)
#define SMOKE_SENSOR_PIN 34 // Smoke sensor (MQ-2)
#define RELAY_PIN 27       // Relay Pin for controlling the water pump

// Wi-Fi credentials
const char* ssid = "ABRGhamry";     
const char* pass = "ghsam9999";  

// OLED Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Fire and Smoke sensor thresholds 
int fireThreshold = 2500;  // Fire sensor threshold 
int smokeThreshold = 1500;  // Smoke sensor threshold 

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
unsigned long lastCheckTime = 0;  // Time of the last check
unsigned long alarmStartTime = 0;  // Start time of the alarm
bool alarmActive = false;  // Flag to track if alarm is active

void setup() {
  Serial.begin(115200);  // Start serial communication

  // Initialize OLED display (using I2C address 0x3C)
  Serial.println("Initializing OLED display...");
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

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);  // Set the relay pin as output

  // Start with the first Green LED ON 
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(RELAY_PIN, HIGH);  //  water pump is OFF by default

  // Initialize all readings to 0
  for (int i = 0; i < NUM_READINGS; i++) {
    fireReadings[i] = 0;
    smokeReadings[i] = 0;
  }

  // Initialize Wi-Fi and Blynk connection
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, pass);  // Connect to Wi-Fi

  int attempts = 0;  // Connection attempts counter
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {  // Timeout after 10 attempts
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to Wi-Fi");
  } else {
    Serial.println("Failed to connect to Wi-Fi");
    return;  // If Wi-Fi connection fails, stop setup here
  }

  // Attempt Blynk connection
  Serial.println("Connecting to Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);  // Connect to Blynk server

  attempts = 0;  // Reset connection attempts counter
  while (!Blynk.connected() && attempts < 10) {  // Timeout after 10 attempts
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  if (Blynk.connected()) {
    Serial.println("Connected to Blynk");
  } else {
    Serial.println("Failed to connect to Blynk");
    return;  // If Blynk connection fails, stop setup here
  }
}

void loop() {
  Blynk.run();  // Run Blynk background tasks

  unsigned long currentMillis = millis();

  // Always update the sensor values continuously
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

  bool fireDetected = false;
  bool smokeDetected = false;

  // Check for fire detection
  if (fireAverage >= fireThreshold) {
    fireDetected = true;
    digitalWrite(RED_LED_PIN, HIGH);    // Red LED ON
    digitalWrite(GREEN_LED_PIN, LOW);   // Green LED OFF
    digitalWrite(BUZZER_PIN, LOW);     // Buzzer ON
    digitalWrite(RELAY_PIN, LOW);       // Activate the relay (turn on the water pump)
    display.print("Fire Detected! Water Pump Activated.");
    alarmStartTime = currentMillis;  // Reset the alarm start time
    alarmActive = true;
  }

  // Check for smoke detection
  if (smokeAverage >= smokeThreshold) {
    smokeDetected = true;
    digitalWrite(RED_LED_PIN, HIGH);    // Red LED ON
    digitalWrite(GREEN_LED_PIN, LOW);   // Green LED OFF
    digitalWrite(BUZZER_PIN, LOW);     // Buzzer ON
    digitalWrite(RELAY_PIN, LOW);       // Activate the relay (turn on the water pump)
    display.print("Smoke Detected! Water Pump Activated.");
    alarmStartTime = currentMillis;  // Reset the alarm start time
    alarmActive = true;
  }

  // If neither fire nor smoke is detected
  if (!fireDetected && !smokeDetected) {
    digitalWrite(RED_LED_PIN, LOW);     // Red LED OFF
    digitalWrite(BUZZER_PIN, HIGH);      // Buzzer OFF
    digitalWrite(GREEN_LED_PIN, HIGH);  // Green LED ON
    digitalWrite(RELAY_PIN, HIGH);      // Deactivate the relay (turn off the water pump)
    display.print("No Fire/Smoke Detected. Water Pump OFF.");
  }

  // Reset the alarm after 10 seconds if active
  if (alarmActive && (currentMillis - alarmStartTime >= 10000)) {
    alarmActive = false;  // Reset alarm state
    digitalWrite(RED_LED_PIN, LOW);      // Red LED OFF
    digitalWrite(BUZZER_PIN, HIGH);       // Buzzer OFF
    digitalWrite(GREEN_LED_PIN, HIGH);   // Green LED ON
    digitalWrite(RELAY_PIN, HIGH);       //  water pump is OFF
    Serial.println("Alarm ended.");
  }

  // Send sensor values to Blynk
  Blynk.virtualWrite(V0, fireAverage);  // Fire sensor value on V0
  Blynk.virtualWrite(V1, smokeAverage); // Smoke sensor value on V1

  // Send relay control status (Water pump) to Blynk
  if (digitalRead(RELAY_PIN) == LOW) {
    Blynk.virtualWrite(V2, 1);  // Water pump ON
  } else {
    Blynk.virtualWrite(V2, 0);  // Water pump OFF
  }

  display.setCursor(0, 20);
  display.print("Fire Value: ");
  display.println(fireAverage);
  display.setCursor(0, 40);
  display.print("Smoke Value: ");
  display.println(smokeAverage);

  display.display();  // Update OLED display
  delay(100);  // Short delay to allow for faster display updates
}
