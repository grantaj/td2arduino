
// Number of Analog sensor inputs
const int NUM_SENSORS = 6;
const unsigned long reportingInterval = 1000; // millisecond
const unsigned long readingInterval = 100; // millisecond

int sensors[NUM_SENSORS] = {A0, A1, A2, A3, A4, A5};
unsigned long sensorReadings = 0;
unsigned long sensorValues[NUM_SENSORS];
unsigned long nextReport;
unsigned long nextRead;

// Define the number of digital pins
const int NUM_PINS = 5;

// Define the digital pins to control
int pins[NUM_PINS] = {2, 3, 4, 5, 6};

// Define the corresponding strings to match
String strings_on[NUM_PINS] = {"red_on", "green_on", "blue_on", "yellow_on", "white_on"};
String strings_off[NUM_PINS] = {"red_off", "green_off", "blue_off", "yellow_off", "white_off"};

// Group control
String all_on = "all_on";
String all_off = "all_off";

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);

  // Set digital pins as output
  for (int i = 0; i < NUM_PINS; i++) {
    pinMode(pins[i], OUTPUT);
  }

  for (int i=0; i < NUM_SENSORS; i++) {
    sensorValues[i]=0;
  }

  nextReport = millis() + reportingInterval;
  nextRead = millis() + readingInterval;
}

void loop() {

  // Analog sensor handler
  unsigned long now = millis();

  if (now > nextRead) {
    for (int i=0; i<NUM_SENSORS; i++) {
     sensorValues[i] += analogRead(sensors[i]);
   }
    sensorReadings++;
    nextRead = millis() + readingInterval;
  }

  if (now > nextReport) {
    for (int i =0; i<NUM_SENSORS; i++){
      Serial.print(sensorValues[i]/sensorReadings);
      Serial.print(" ");
      sensorValues[i] = 0;
    }
    Serial.print("\n");

    sensorReadings = 0;
    nextReport = millis() + reportingInterval;
  }


  // Button illumination handler
  if (Serial.available()) {
    // Read the line from the serial port
    String input = Serial.readStringUntil('\n');
    input.trim();  // Remove leading/trailing whitespaces

    if (input==all_on) {
      for (int i = 0; i < NUM_PINS; i++) {
        digitalWrite(pins[i], HIGH);
      }
      return;
    }

    if (input == all_off) {
      for (int i = 0; i < NUM_PINS; i++) {
        digitalWrite(pins[i], LOW);
      }
      return;
    }

    // Check for a match in the array of strings
    for (int i = 0; i < NUM_PINS; i++) {
      if (input == strings_on[i]) {
        // Set the corresponding digital pin high
        digitalWrite(pins[i], HIGH);
        break;  // Exit the loop once a match is found
      }
      if (input == strings_off[i]) {
        digitalWrite(pins[i], LOW);
        break;
      }
    }
  }
}