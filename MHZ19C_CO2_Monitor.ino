#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define pins for software serial communication
#define RX_PIN 2      // Connect to TX pin of MH-Z19C
#define TX_PIN 3      // Connect to RX pin of MH-Z19C
#define RELAY_PIN 4   // Connect to relay signal pin

// CO2 threshold levels in ppm
const int CO2_GOOD_THRESHOLD = 800;    // Below this is considered good
const int CO2_FAIR_THRESHOLD = 1200;   // Below this is considered fair, above is poor
const int CO2_SENSOR_WARMUP_TIME = 10000;    // Time for the sensor to warm up

// Initialize LCD (0x27 is the xdefault I2C address, adjust if needed)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display

SoftwareSerial mhzSerial(RX_PIN, TX_PIN);  // Create software serial object

byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
unsigned char response[9];  // Array to store the response

void setup() {
  Serial.begin(9600);      // Start hardware serial for debugging
  mhzSerial.begin(9600);   // Start software serial for sensor communication
  
  // Initialize relay pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Start with fan off
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CO2 Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Warming up...");
  
  Serial.println("MH-Z19C CO2 Sensor Initialization...");
  delay(CO2_SENSOR_WARMUP_TIME);  // Warm up time for the sensor
}

void loop() {
  int CO2ppm = readCO2();
  
  if (CO2ppm > 0) {
    // Update Serial output
    Serial.print("CO2 Concentration: ");
    Serial.print(CO2ppm);
    Serial.println(" ppm");
    
    // Update LCD display
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CO2 Level:");
    lcd.setCursor(0, 1);
    lcd.print(CO2ppm);
    lcd.print(" ppm");
    
    // Control fan and display air quality
    lcd.setCursor(11, 1);
    if (CO2ppm <= CO2_GOOD_THRESHOLD) {
      lcd.print("GOOD");
      digitalWrite(RELAY_PIN, LOW);  // Turn off fan
      Serial.println("Fan: OFF");
    } else if (CO2ppm <= CO2_FAIR_THRESHOLD) {
      lcd.print("FAIR");
      digitalWrite(RELAY_PIN, HIGH); // Turn on fan
      Serial.println("Fan: ON");
    } else {
      lcd.print("POOR");
      digitalWrite(RELAY_PIN, HIGH); // Turn on fan
      Serial.println("Fan: ON");
    }
  } else {
    Serial.println("Failed to read from sensor");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error");
    lcd.setCursor(0, 1);
    lcd.print("Check Connection");
    digitalWrite(RELAY_PIN, LOW);  // Turn off fan on error
  }
  
  delay(2000);  // Wait 2 seconds before next reading
}

int readCO2() {
  // Send command to read CO2
  mhzSerial.write(cmd, 9);
  
  // Wait for response
  if (mhzSerial.available() >= 9) {
    for (int i = 0; i < 9; i++) {
      response[i] = mhzSerial.read();
    }
    
    // Verify checksum
    byte check = getChecksum(response);
    if (response[8] != check) {
      Serial.println("Checksum error!");
      return -1;
    }
    
    // Calculate CO2 concentration
    int responseHigh = (int) response[2];
    int responseLow = (int) response[3];
    int ppm = (responseHigh << 8) + responseLow;
    
    return ppm;
  }
  
  return -1;
}

byte getChecksum(unsigned char* packet) {
  byte checksum = 0;
  for (int i = 1; i < 8; i++) {
    checksum += packet[i];
  }
  checksum = 0xff - checksum;
  checksum += 1;
  return checksum;
} 