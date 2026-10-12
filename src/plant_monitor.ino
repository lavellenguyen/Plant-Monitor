#include <Wire.h>              
#include <LiquidCrystal_I2C.h>  


LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initializes LCD at address 0x27 for a 16x2 display


const int lightSensorPin = A0;  // Light sensor
const int moisturePin = A1;     // Soil moisture sensor
const int tempSensorPin = A2;   // NTC thermistor
const int pumpPin = 12;         // Pump


// Constants for temperature calculation
const float beta = 3950.0;               // NTC thermistor's Beta value
const float seriesResistor = 10000;      // Series resistor value (ohms)
const float roomTempResistance = 10000;  // NTC resistance at 25°C
const float roomTemp = 25 + 273.15;      // Room temperature in Kelvin


void setup() {
  pinMode(pumpPin, OUTPUT);  // Sets the pump pin as output
  lcd.init();                // Initializes LCD display
  lcd.backlight();           // Turns on LCD backlight for visibility
}


void loop() {
  // Read sensors
  int tempValue = analogRead(tempSensorPin);
  int lightValue = analogRead(lightSensorPin);
  int moistureValue = analogRead(moisturePin);


  // Calculate soil moisture percentage
  float moisturePercent = map(moistureValue, 0, 1023, 100, 0);


  // Calculate temperature in Celsius
  float resistance = (1023.0 / tempValue - 1) * seriesResistor;
  float tempK = 1 / (log(resistance / roomTempResistance) / beta + 1 / roomTemp);
  float tempC = tempK - 273.15;
  float tempF = tempC * 9.0 / 5.0 + 32.0;


  // Display Temperature
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp C: ");
  lcd.print(tempC);
  lcd.setCursor(0, 1);
  lcd.print("Temp F: ");
  lcd.print(tempF);
  delay(2000);


  // Display light and soil moisture
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Light: ");
  lcd.print(lightValue);
  lcd.setCursor(0, 1);
  lcd.print("Soil: ");
  lcd.print(moisturePercent);
  lcd.print("%");
  delay(2000);


  // Control pump if soil moisture is below 35%
  if (moisturePercent < 35) {
    digitalWrite(pumpPin, HIGH);  // Turn on pump
    delay(3000);
    digitalWrite(pumpPin, LOW);  // Turn off pump
    lcd.init(); // Reinitialize LCD to prevent display corruption
  }
}
