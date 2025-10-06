/*
  ESP32 Advanced Blink LED with OTA Support
  
  This example demonstrates LED blinking with additional features suitable for OTA projects:
  - Configurable blink patterns
  - Serial communication for monitoring
  - LED status indicators for different states
  
  Hardware:
  - ESP32 Development Board
  - Built-in LED connected to GPIO 2 (or external LED)
  
  Features:
  - Basic LED blinking
  - Configurable timing
  - Serial output for debugging
  - Multiple blink patterns
  
  Created for ESP32-OTA repository
  This example code is in the public domain.
*/

// LED Configuration
#define LED_PIN 2           // Built-in LED pin (adjust for your board)
#define BLINK_INTERVAL 1000 // Default blink interval in milliseconds

// Blink patterns
enum BlinkPattern {
  PATTERN_NORMAL,     // Standard on/off blink
  PATTERN_FAST,       // Fast blinking
  PATTERN_SLOW,       // Slow blinking
  PATTERN_HEARTBEAT,  // Double blink pattern
  PATTERN_SOS         // SOS pattern (... --- ...)
};

// Global variables
BlinkPattern currentPattern = PATTERN_NORMAL;
unsigned long previousMillis = 0;
bool ledState = false;
int patternStep = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Wait for serial port to connect
  delay(1000);
  
  // Print startup information
  Serial.println("\n=== ESP32 Advanced Blink LED ===");
  Serial.println("Version: 1.0");
  Serial.println("LED Pin: GPIO " + String(LED_PIN));
  Serial.println("Default Interval: " + String(BLINK_INTERVAL) + "ms");
  Serial.println("=====================================\n");
  
  // Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
  
  // Start with LED off
  digitalWrite(LED_PIN, LOW);
  ledState = false;
  
  Serial.println("Setup complete!");
  Serial.println("Available commands:");
  Serial.println("  '1' - Normal blink");
  Serial.println("  '2' - Fast blink");
  Serial.println("  '3' - Slow blink");
  Serial.println("  '4' - Heartbeat pattern");
  Serial.println("  '5' - SOS pattern");
  Serial.println("  'i' - Show info");
  Serial.println();
  
  printCurrentPattern();
}

void loop() {
  // Check for serial commands
  checkSerialCommands();
  
  // Execute current blink pattern
  executeBlinkPattern();
}

void checkSerialCommands() {
  if (Serial.available()) {
    char command = Serial.read();
    
    switch (command) {
      case '1':
        currentPattern = PATTERN_NORMAL;
        patternStep = 0;
        Serial.println("Pattern changed to: Normal");
        break;
        
      case '2':
        currentPattern = PATTERN_FAST;
        patternStep = 0;
        Serial.println("Pattern changed to: Fast");
        break;
        
      case '3':
        currentPattern = PATTERN_SLOW;
        patternStep = 0;
        Serial.println("Pattern changed to: Slow");
        break;
        
      case '4':
        currentPattern = PATTERN_HEARTBEAT;
        patternStep = 0;
        Serial.println("Pattern changed to: Heartbeat");
        break;
        
      case '5':
        currentPattern = PATTERN_SOS;
        patternStep = 0;
        Serial.println("Pattern changed to: SOS");
        break;
        
      case 'i':
      case 'I':
        printSystemInfo();
        break;
        
      default:
        Serial.println("Unknown command: " + String(command));
        break;
    }
  }
}

void executeBlinkPattern() {
  unsigned long currentMillis = millis();
  unsigned long interval = BLINK_INTERVAL;
  
  switch (currentPattern) {
    case PATTERN_NORMAL:
      interval = 1000;
      if (currentMillis - previousMillis >= interval) {
        toggleLED();
        previousMillis = currentMillis;
      }
      break;
      
    case PATTERN_FAST:
      interval = 200;
      if (currentMillis - previousMillis >= interval) {
        toggleLED();
        previousMillis = currentMillis;
      }
      break;
      
    case PATTERN_SLOW:
      interval = 2000;
      if (currentMillis - previousMillis >= interval) {
        toggleLED();
        previousMillis = currentMillis;
      }
      break;
      
    case PATTERN_HEARTBEAT:
      executeHeartbeatPattern(currentMillis);
      break;
      
    case PATTERN_SOS:
      executeSOSPattern(currentMillis);
      break;
  }
}

void executeHeartbeatPattern(unsigned long currentMillis) {
  // Heartbeat: quick double blink, then pause
  unsigned long intervals[] = {100, 100, 100, 100, 1000}; // on, off, on, off, pause
  
  if (currentMillis - previousMillis >= intervals[patternStep % 5]) {
    if (patternStep % 2 == 0) {
      setLED(true);  // Turn on for even steps (0, 2)
    } else {
      setLED(false); // Turn off for odd steps (1, 3, 4)
    }
    
    previousMillis = currentMillis;
    patternStep = (patternStep + 1) % 5;
  }
}

void executeSOSPattern(unsigned long currentMillis) {
  // SOS: ... --- ... (3 short, 3 long, 3 short)
  unsigned long shortBlink = 200;
  unsigned long longBlink = 600;
  unsigned long pause = 200;
  unsigned long longPause = 1000;
  
  // Pattern: S(3 short) O(3 long) S(3 short) pause
  unsigned long intervals[] = {
    shortBlink, pause, shortBlink, pause, shortBlink, longPause, // S
    longBlink, pause, longBlink, pause, longBlink, longPause,   // O
    shortBlink, pause, shortBlink, pause, shortBlink, longPause // S
  };
  
  if (currentMillis - previousMillis >= intervals[patternStep % 18]) {
    // Turn on LED for blink intervals (0,2,4,6,8,10,12,14,16)
    if (patternStep % 2 == 0 && patternStep < 16) {
      setLED(true);
    } else {
      setLED(false);
    }
    
    previousMillis = currentMillis;
    patternStep = (patternStep + 1) % 18;
  }
}

void toggleLED() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  Serial.println("LED " + String(ledState ? "ON" : "OFF"));
}

void setLED(bool state) {
  if (ledState != state) {
    ledState = state;
    digitalWrite(LED_PIN, state ? HIGH : LOW);
    // Only print for visible changes to reduce serial spam
    if (currentPattern == PATTERN_NORMAL || currentPattern == PATTERN_FAST || currentPattern == PATTERN_SLOW) {
      Serial.println("LED " + String(state ? "ON" : "OFF"));
    }
  }
}

void printCurrentPattern() {
  Serial.print("Current pattern: ");
  switch (currentPattern) {
    case PATTERN_NORMAL:
      Serial.println("Normal (1s interval)");
      break;
    case PATTERN_FAST:
      Serial.println("Fast (200ms interval)");
      break;
    case PATTERN_SLOW:
      Serial.println("Slow (2s interval)");
      break;
    case PATTERN_HEARTBEAT:
      Serial.println("Heartbeat (double blink)");
      break;
    case PATTERN_SOS:
      Serial.println("SOS (... --- ...)");
      break;
  }
}

void printSystemInfo() {
  Serial.println("\n=== System Information ===");
  Serial.println("Chip Model: " + String(ESP.getChipModel()));
  Serial.println("Chip Revision: " + String(ESP.getChipRevision()));
  Serial.println("CPU Frequency: " + String(ESP.getCpuFreqMHz()) + " MHz");
  Serial.println("Free Heap: " + String(ESP.getFreeHeap()) + " bytes");
  Serial.println("Flash Size: " + String(ESP.getFlashChipSize()) + " bytes");
  Serial.println("Uptime: " + String(millis() / 1000) + " seconds");
  Serial.print("LED Pin: GPIO ");
  Serial.print(LED_PIN);
  Serial.println(" (State: " + String(ledState ? "ON" : "OFF") + ")");
  printCurrentPattern();
  Serial.println("===========================\n");
}