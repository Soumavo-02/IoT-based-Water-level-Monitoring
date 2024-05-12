// C++ code
//
#include <Adafruit_LiquidCrystal.h>

int dist = 0;

Adafruit_LiquidCrystal lcd_1(0);

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void setup()
{
  lcd_1.begin(16, 2);
}

void loop()
{
  dist = map(0.01723 * readUltrasonicDistance(2, 3), 2.9, 336, 100, 0);
  lcd_1.setCursor(0, 0);
  lcd_1.print("Water Level:");
  lcd_1.setCursor(0, 1);
  lcd_1.print(dist);
  lcd_1.print("%");
  if(dist >=80)
  {
    digitalWrite(8,HIGH);
    tone(10,523,10000);
    delay(10000);
    digitalWrite(11,HIGH);
    digitalWrite(12,HIGH);
    delay(15000);
    
  }
  else if (dist <=30)
  {
    digitalWrite(8,HIGH);
    tone(10,523,10000);
    delay(10000);
    digitalWrite(11,HIGH);
    digitalWrite(12,LOW);
    delay(15000);
    
  
  }
  else {
    noTone(10);
    digitalWrite(8,LOW);
    
   
  }
    
    
    
  delay(10);// Delay a little bit to improve simulation performance
}