#ifndef MAIN_H
#define MAIN_H

#define PIN_FAN 17  // Digital output pin to control the fan

#define PIN_THERMO_DO 19   // Data Out pin for the MAX6675 thermocouple
#define PIN_THERMO_CS 5    // Chip Select pin for the MAX6675 thermocouple
#define PIN_THERMO_CLK 18  // Clock pin for the MAX6675 thermocouple
#define PIN_THERMO2_CS 16

#define TFT_CS 15   // Chip Select pin for the TFT display
#define TFT_DC 22   // Data/Command control pin for the TFT display
#define TFT_RST 4   // Reset pin for the TFT display
#define TFT_MOSI 23 // Master Out Slave In pin for SPI communication
#define TFT_SCLK 18 // Serial Clock pin for SPI communication

enum TemperatureUnit {
    CELSIUS,
    FAHRENHEIT
};


// Function Prototypes
void initializeHardware();
void initializeTFT();
void handleSerialCommand();
float readTemperature(TemperatureUnit unit, int sensor);
void updateTFT(float temp, bool fanState);

#endif // MAIN_H
