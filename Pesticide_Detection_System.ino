#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const int AO_Pin = A0; // Connect the AO of MQ-4 sensor with analog channel 0 pin (A0) of Arduino
const int DO_Pin = 8; // Connect the DO of MQ-4 sensor with digital pin 8 (D8) of Arduino

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

int threshold; // Create a variable to store the digital output of the MQ-4 sensor
int AO_Out; // Create a variable to store the analog output of the MQ-4 sensor

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16x2 display

// Function to calculate percentage of pesticide present
float calculatePesticidePercentage(int methaneValue) {
  // Assuming a linear relationship between methane concentration and pesticide percentage
  // You may need to adjust this based on the characteristics of your sensor and the pesticide being detected
  // Map the range of methane values (0-1023) to the percentage of pesticide present (0-100)
  return map(methaneValue, 0, 1023, 0, 100);
}

// Set up
void setup() {
  Serial.begin(115200);  // Initialize serial monitor using a baud rate of 115200
  pinMode(DO_Pin, INPUT); // Set the D8 pin as a digital input pin
  dht.begin();

  lcd.begin(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
}

// Main loop
void loop() {
  AO_Out = analogRead(AO_Pin); // Read the analog output measurement sample from the MQ4 sensor's AO pin
  threshold = digitalRead(DO_Pin); // Read the digital output of MQ-4 sensor's DO pin
  
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Calculate percentage of pesticide present
  float pesticidePercentage = calculatePesticidePercentage(AO_Out);

  lcd.clear(); // Clear the LCD display
  lcd.setCursor(0, 0); // Set cursor to the beginning of the first line
  lcd.print("Threshold: "); // Print out the text "Threshold: "
  lcd.print(threshold ? "HIGH" : "LOW"); // Print the threshold reached - with will either print be LOW or HIGH (above or underneath)

  lcd.setCursor(0, 1); // Set cursor to the beginning of the second line
  lcd.print("Methane: "); // Print out the text "Methane Concentration: "
  lcd.print(AO_Out); // Print out the methane value - the analog output - between 0 and 1023
  delay(2000); 
  lcd.clear(); 

  lcd.setCursor(0, 1);
  lcd.print("Pesticide: ");
  lcd.print(pesticidePercentage);
  lcd.print("%");
  delay(2000); 
  lcd.clear(); 

  lcd.setCursor(0, 1);
  lcd.print("Temperature:");
  lcd.print(temperature);
  lcd.print("C");
  delay(2000); 
  lcd.clear(); 

  lcd.setCursor(0, 3);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
  delay(2000); 
  lcd.clear(); 
}