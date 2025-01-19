#include <max6675.h>
#include <Adafruit_GFX.h>  // Core graphics library
#include <Adafruit_ST7789.h>
#include <SPI.h>  // Arduino SPI library

// Pin Definitions for Thermocouple
#define PIN_THERMO_DO 19   // Data Out pin for the MAX6675 thermocouple
#define PIN_THERMO_CS 5    // Chip Select pin for the MAX6675 thermocouple
#define PIN_THERMO_CLK 18  // Clock pin for the MAX6675 thermocouple

// Pin Definition for Fan Control
#define PIN_FAN 17         // Digital output pin to control the fan

// Pin Definitions for TFT Display
#define TFT_CS 15          // Chip Select pin for the TFT display
#define TFT_DC 22          // Data/Command control pin for the TFT display
#define TFT_RST 4          // Reset pin for the TFT display
#define TFT_MOSI 23        // Master Out Slave In pin for SPI communication
#define TFT_SCLK 18        // Serial Clock pin for SPI communication

// TFT Object
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Temperature Unit Setting
enum TemperatureUnit { CELSIUS,
                       FAHRENHEIT };
TemperatureUnit tempUnit = CELSIUS;

// Thermocouple Object
MAX6675 thermocouple(PIN_THERMO_CLK, PIN_THERMO_CS, PIN_THERMO_DO);

// Constants
const unsigned int LOOP_DELAY_MS = 50;  // Main loop delay

// Function Prototypes
void initializeHardware();
void initializeTFT();
void handleSerialCommand();
void sendTemperatureData();
float readTemperature(TemperatureUnit unit);
void controlFan(bool state);
void updateTFT(float temp, bool fanState);

// Entry Point
void setup() {
  Serial.begin(115200);
  Serial.println("Artisan Roasting Software Initialized");
  initializeHardware();
  initializeTFT();
}

void loop() {
  handleSerialCommand();

  // Periodically read temperature and update the TFT
  float currentTemp = readTemperature(tempUnit);
  bool fanState = digitalRead(PIN_FAN);
  updateTFT(currentTemp, fanState);

  delay(LOOP_DELAY_MS);
}

// Initializes hardware components
void initializeHardware() {
  pinMode(PIN_FAN, OUTPUT);
  controlFan(false);  // Ensure fan is off initially

  pinMode(PIN_THERMO_CS, OUTPUT);     // Ensure thermocouple CS pin is output
  digitalWrite(PIN_THERMO_CS, HIGH);  // Disable thermocouple initially
}

// Initializes the TFT display
void initializeTFT() {
  tft.init(240, 320, SPI_MODE2);  // Initialize with the resolution of the display
  tft.setRotation(1);             // Landscape mode
  tft.fillScreen(ST77XX_BLACK);

  // Title
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(20, 10);
  tft.print("Mehmet's Roaster");

  // Labels
  tft.setTextSize(2);
  tft.setCursor(20, 70);
  tft.print("Temp:");
  tft.setCursor(20, 120);
  tft.print("Fan:");
  tft.setCursor(20, 170);
  tft.print("Status:");
}

// Reads temperature from the thermocouple
float readTemperature(TemperatureUnit unit) {
  digitalWrite(TFT_CS, HIGH);        // Disable TFT
  digitalWrite(PIN_THERMO_CS, LOW);  // Enable thermocouple

  float temp = unit == FAHRENHEIT ? thermocouple.readFahrenheit() : thermocouple.readCelsius();

  digitalWrite(PIN_THERMO_CS, HIGH);  // Disable thermocouple
  digitalWrite(TFT_CS, LOW);          // Enable TFT

  if (isnan(temp)) {
    Serial.println("Error: Temperature read failed!");
    return -1.0;  // Indicates an error
  }
  return temp;
}

// Updates the TFT with dynamic values
void updateTFT(float temp, bool fanState) {
  // Temperature
  tft.fillRect(120, 70, 160, 30, ST77XX_BLACK);  // It won't display properly if this isn't done twice
  tft.fillRect(120, 70, 160, 30, ST77XX_BLACK);  // Clear previous temp
  tft.setCursor(120, 70);
  if (temp < 0) {
    tft.setTextColor(ST77XX_RED);
    tft.print("Error");
  } else {
    tft.setTextColor(temp > 100 ? ST77XX_RED : ST77XX_GREEN);
    tft.print(temp);
    tft.print(tempUnit == CELSIUS ? " C" : " F");
  }

  // Fan Status
  tft.fillRect(120, 120, 160, 30, ST77XX_BLACK);  // Clear previous fan status
  tft.setCursor(120, 120);
  tft.setTextColor(fanState ? ST77XX_GREEN : ST77XX_RED);
  tft.print(fanState ? "ON" : "OFF");

  // Status
  tft.fillRect(120, 170, 200, 30, ST77XX_BLACK);  // Clear previous status
  tft.setCursor(120, 170);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Running...");
}

// Handles incoming serial commands
void handleSerialCommand() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();  // Remove whitespace

    if (command.startsWith("CHAN;")) {
      Serial.println("#OK");  // Placeholder, no specific action
    } else if (command.startsWith("UNITS;")) {
      char unit = command.charAt(6);
      if (unit == 'F') {
        tempUnit = FAHRENHEIT;
        Serial.println("#OK Fahrenheit");
      } else if (unit == 'C') {
        tempUnit = CELSIUS;
        Serial.println("#OK Celsius");
      } else {
        Serial.println("#ERROR: Invalid Unit");
      }
    } else if (command.startsWith("READ")) {
      sendTemperatureData();
    } else if (command.startsWith("OT1;")) {
      char state = command.charAt(4);
      if (state == '0') {
        controlFan(false);
        Serial.println("#OK Fan OFF");
      } else if (state == '1') {
        controlFan(true);
        Serial.println("#OK Fan ON");
      } else {
        Serial.println("#ERROR: Invalid Fan State");
      }
    } else {
      Serial.println("#ERROR: Unknown Command");
    }
  }
}

// Sends temperature data in a formatted string
void sendTemperatureData() {
  float temp = readTemperature(tempUnit);
  if (temp < 0) {
    Serial.println("#ERROR: Invalid Temperature Data");
    return;
  }
  Serial.print("30.00,");
  Serial.print(temp);
  Serial.print(",");
  Serial.print(tempUnit == FAHRENHEIT ? "150.00" : "25.00");
  Serial.println(",10.00,20.00");
}

// Controls the fan state
void controlFan(bool state) {
  digitalWrite(PIN_FAN, state ? HIGH : LOW);
}
