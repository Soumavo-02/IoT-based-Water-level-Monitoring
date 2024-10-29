#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#define BLYNK_TEMPLATE_ID "TMPL3syF5aA18"
#define BLYNK_TEMPLATE_NAME "Water Level Monitoring"
#define BLYNK_AUTH_TOKEN "9QS4JkuJGasrsgmQtoMxAGo1NxUQ0nwu"
#include <BlynkSimpleEsp8266.h>

// Pin definitions
#define GREEN_LED D3 // GPIO0
#define RED_LED D4   // GPIO2
#define TRIG_PIN D6  // GPIO12
#define ECHO_PIN D7  // GPIO13
#define RELAY_PIN D5 // GPIO14


// Initialize the LCD display
LiquidCrystal_I2C lcd=LiquidCrystal_I2C(0x27,16,2);

// WiFi credentials
char auth[] = "9QS4JkuJGasrsgmQtoMxAGo1NxUQ0nwu"; // Enter your Auth token
char ssid[] = "Soumavo 4G";
char pass[] = "Soumavo@2002"; // Enter your WIFI password

BlynkTimer timer;

const int maxLevel = 18; // Max water level in cm
int waterLevelPercentage = 0;

void setup() {
  // Debug console
  Serial.begin(115200);
  
  // Initialize Blynk
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  
  // Initialize the LCD
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();

  // Initialize pin modes
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Set relay off initially
  digitalWrite(RELAY_PIN, HIGH);

  // Display system loading message
  lcd.setCursor(0, 0);
  lcd.print("System");
  lcd.setCursor(4, 1);
  lcd.print("Loading..");
  delay(2000);
  lcd.clear();

  // Setup a function to be called every second
  timer.setInterval(1000L, measureWaterLevel);
}

void measureWaterLevel() {
  // Trigger the ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pin
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2; // Calculate the distance in cm

  // Calculate water level percentage
  int waterLevel = maxLevel - distance;
  waterLevelPercentage = (waterLevel * 100) / maxLevel;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Water Level: ");
  Serial.print(waterLevel);
  Serial.print(" cm (");
  Serial.print(waterLevelPercentage);
  Serial.println("%)");

  // Update Blynk gauge widget
  Blynk.virtualWrite(V0, waterLevelPercentage);

  // Update LCD display
  lcd.setCursor(0, 0);
  lcd.print("Water Level: ");
  lcd.setCursor(12,0);
  lcd.print(waterLevelPercentage);
  lcd.print("%      "); // Clear any remaining characters

  // Control LEDs based on water level percentage
  if (waterLevelPercentage < 25 ) {
    digitalWrite(RED_LED, HIGH);
  } 
  else if(waterLevelPercentage >80){
    digitalWrite(RED_LED, HIGH);
  }
  else {
    digitalWrite(RED_LED, LOW);
  }
}

BLYNK_WRITE(V1) {
  bool relayState = param.asInt();
  if (relayState) {
    digitalWrite(RELAY_PIN, HIGH); // Turn relay on
    digitalWrite(GREEN_LED, HIGH); // Turn green LED on
    lcd.setCursor(0, 1);
    lcd.print("Motor: ON ");
  } else {
    digitalWrite(RELAY_PIN, LOW); // Turn relay off
    digitalWrite(GREEN_LED, LOW); // Turn green LED off
    lcd.setCursor(0, 1);
    lcd.print("Motor: OFF");
  }
}

void loop() {
  Blynk.run(); // Run the Blynk library
  timer.run(); // Run the timer
}
