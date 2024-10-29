// Include necessary libraries
#include <DHT11.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Sets interrupt button to pin 3
const byte interruptPin = 3;
volatile byte state = LOW;

// Create an instance of the DHT11 and liquid crystal class.
DHT11 dht11(2);
  
LiquidCrystal_I2C lcd(0x27,20,4);

// Global variable to track when the button was last pressed
unsigned long lastPressed = 0;

void setup() {
  // Initialize serial communication to allow debugging and data readout.
  // Using a baud rate of 9600 bps.
  Serial.begin(9600);
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  // Relay Pin
  pinMode(4, OUTPUT);

  // Set up for interrupt button
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), backlight, RISING);
}

void loop() {
    int temperature = 0;
    int humidity = 0;

    // Attempt to read the temperature and humidity values from the DHT11 sensor.
    int result = dht11.readTemperatureHumidity(temperature, humidity);

    // Check the results of the readings.
    // If the reading is successful, print the temperature and humidity values.
    // If there are errors, print the appropriate error messages.
    if (result == 0) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" °C\tHumidity: ");
        Serial.print(humidity);
        Serial.println(" %");

        // Prints the temperature to the display
        lcd.setCursor(0,0);
        lcd.print("Temperature:");
        lcd.setCursor(13,0);
        lcd.print(temperature);

        // Turns on relay if humidity is greater than 60
        if (humidity >= 60) {
          Serial.println("Turning Relay ON");
          digitalWrite(4, HIGH);  // Turn relay ON if humidity >= 60
        }
        else {
            digitalWrite(4, LOW);   // Turn relay OFF if humidity < 60
        }
        
        lcd.setCursor(2,1);
        lcd.print("Humidity: ");
        lcd.setCursor(12,1);
        lcd.print(humidity);
        lcd.setCursor(14,1);
        lcd.print("%");
        
    } else {
        // Print error message based on the error code.
        Serial.println(DHT11::getErrorString(result));
    }

    // Error checking
    Serial.println(lastPressed);
    // Sees if it has been more then 5 seconds since button was pressed
    if (millis() > lastPressed + 5000) {
      lcd.noBacklight(); // turn off backlight
    }
    else {
      lcd.backlight(); // turn on backlight.
    }
}

void backlight() {
  lastPressed = millis();
}
