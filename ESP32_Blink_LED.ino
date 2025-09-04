/*
  ESP32 Blink LED
  
  This example shows the simplest thing you can do with an ESP32 to see physical output:
  it blinks the on-board LED.
  
  Hardware:
  - ESP32 Development Board
  - Built-in LED connected to GPIO 2 (varies by board)
  
  The circuit:
  - LED attached from GPIO 2 to ground through built-in resistor
  
  Created for ESP32-OTA repository
  This example code is in the public domain.
*/

// Define the LED pin - GPIO 2 is commonly used for built-in LED on ESP32
#define LED_PIN 2

// Alternative pins if your board uses a different pin:
// #define LED_PIN 4   // Some ESP32 boards use GPIO 4
// #define LED_PIN 5   // Some ESP32 boards use GPIO 5
// #define LED_PIN 13  // Some ESP32 boards use GPIO 13

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);
  
  // Print startup message
  Serial.println("ESP32 Blink LED Example");
  Serial.println("LED Pin: " + String(LED_PIN));
  
  // Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
  
  // Start with LED off
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("Setup complete. LED blinking will start now...");
}

void loop() {
  // Turn the LED on
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED ON");
  
  // Wait for 1 second
  delay(1000);
  
  // Turn the LED off
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED OFF");
  
  // Wait for 1 second
  delay(1000);
}