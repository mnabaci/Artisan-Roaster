#include <max6675.h>
#include <Adafruit_GFX.h>  // Core graphics library
#include <Adafruit_ST7789.h>
#include <SPI.h>  // Arduino SPI library

// Commands and commands handler
#include "./main.h"
#include "../lib/CommandHandler/CommandHandler.h"
#include "../lib/Commands/UnitsCommand.h"
#include "../lib/Commands/FanCommand.h"
#include "../lib/Commands/ReadCommand.h"
#include "../lib/Commands/ChanCommand.h"
#include "../lib/Commands/HeatCommand.h"

// TFT Object
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

TemperatureUnit tempUnit = CELSIUS;

// Thermocouple Object
MAX6675 thermocouple(PIN_THERMO_CLK, PIN_THERMO_CS, PIN_THERMO_DO);
MAX6675 thermocouple2(PIN_THERMO_CLK, PIN_THERMO2_CS, PIN_THERMO_DO);

// Constants
const unsigned int LOOP_DELAY_MS = 50;  // Main loop delay

// Command Handler
CommandHandler commandHandler;

// Entry Point
void setup() {
  Serial.begin(115200);
  Serial.println("Artisan Roasting Software Initialized");

  // Register commands. OT2 is Artisan's fan channel; OT1 (heater) has no output on this
  // board and is just acked (see HeatCommand.h).
  commandHandler.registerCommand("UNITS", handleUnitsCommand);
  commandHandler.registerCommand("OT1", handleHeatCommand);
  commandHandler.registerCommand("OT2", handleFanCommand);
  commandHandler.registerCommand("READ", handleReadCommand);
  commandHandler.registerCommand("CHAN", handleChanCommand);

  initializeHardware();
  initializeTFT();
}

void loop() {
  // UBaseType_t freeStack = uxTaskGetStackHighWaterMark(NULL);
  // Serial.print("Free Stack: ");
  // Serial.println(freeStack);

  // Serial.print("Free Heap: ");
  // Serial.println(ESP.getFreeHeap());
  handleSerialCommand();

  // Read temperatures from both sensors
  float temp1 = readTemperature(tempUnit, 1);
  float temp2 = readTemperature(tempUnit, 2);
  bool fanState = digitalRead(PIN_FAN);

  // Update the display with both temperatures and fan state
  updateTFT(temp1, temp2, fanState);

  delay(LOOP_DELAY_MS);
}

// Initializes hardware components
void initializeHardware() {
  pinMode(PIN_FAN, OUTPUT);
  digitalWrite(PIN_FAN, LOW);  // Ensure fan is off initially

  pinMode(PIN_THERMO_CS, OUTPUT);     // Ensure thermocouple 1 CS pin is output
  digitalWrite(PIN_THERMO_CS, HIGH);  // Disable thermocouple 1 initially

  pinMode(PIN_THERMO2_CS, OUTPUT);     // Ensure thermocouple 2 CS pin is output
  digitalWrite(PIN_THERMO2_CS, HIGH);  // Disable thermocouple 2 initially
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
  tft.print("ET:");
  tft.setCursor(20, 110);
  tft.print("BT:");
  tft.setCursor(20, 150);
  tft.print("Fan:");
  tft.setCursor(20, 190);
  tft.print("Status:");
}

void handleSerialCommand() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();  // Remove whitespace
    commandHandler.handleCommand(command);
  }
}

// Reads temperature from the thermocouple
float readTemperature(TemperatureUnit unit, int sensor) {
  int thermo_cs = (sensor == 1) ? PIN_THERMO_CS : PIN_THERMO2_CS;

  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

  digitalWrite(TFT_CS, HIGH);    // Disable TFT
  digitalWrite(thermo_cs, LOW);  // Enable selected thermocouple

  float temp = (unit == FAHRENHEIT)
                 ? (sensor == 1 ? thermocouple.readFahrenheit() : thermocouple2.readFahrenheit())
                 : (sensor == 1 ? thermocouple.readCelsius() : thermocouple2.readCelsius());

  digitalWrite(thermo_cs, HIGH);  // Disable selected thermocouple
  SPI.endTransaction();           // End SPI transaction

  if (isnan(temp)) {
    Serial.print("Error: Temperature read failed for sensor ");
    Serial.println(sensor);
    return -1.0;  // Indicates an error
  }

  return temp;
}

// Updates the TFT with dynamic values
void updateTFT(float temp1, float temp2, bool fanState) {
  digitalWrite(TFT_CS, LOW);  // Enable TFT

  // Sensor 1 Temperature
  tft.fillRect(120, 70, 1, 1, ST77XX_BLACK);     // Clear previous temp
  tft.fillRect(120, 70, 160, 30, ST77XX_BLACK);  // Clear previous temp
  tft.setCursor(120, 70);
  if (temp1 < 0) {
    tft.setTextColor(ST77XX_RED);
    tft.print("Error");
  } else {
    tft.setTextColor(temp1 > 100 ? ST77XX_RED : ST77XX_GREEN);
    tft.print(temp1);
    tft.print(tempUnit == CELSIUS ? " C" : " F");
  }

  // Sensor 2 Temperature
  tft.fillRect(120, 110, 160, 30, ST77XX_BLACK);  // Clear previous temp
  tft.setCursor(120, 110);
  if (temp2 < 0) {
    tft.setTextColor(ST77XX_RED);
    tft.print("Error");
  } else {
    tft.setTextColor(temp2 > 100 ? ST77XX_RED : ST77XX_GREEN);
    tft.print(temp2);
    tft.print(tempUnit == CELSIUS ? " C" : " F");
  }

  // Fan Status
  tft.fillRect(120, 150, 160, 30, ST77XX_BLACK);  // Clear previous fan status
  tft.setCursor(120, 150);
  tft.setTextColor(fanState ? ST77XX_GREEN : ST77XX_RED);
  tft.print(fanState ? "ON" : "OFF");

  // Status
  tft.fillRect(120, 190, 200, 30, ST77XX_BLACK);  // Clear previous status
  tft.setCursor(120, 190);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Running...");

  digitalWrite(TFT_CS, HIGH);
}