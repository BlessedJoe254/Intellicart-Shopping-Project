#include <LiquidCrystal.h>

// Define pins
const int trigPin = 9;
const int echoPin = 10;
const int redLed = 2;
const int yellowLed = 3;
const int greenLed = 4;
const int blueLed = 5;

// Initialize LCD with appropriate pin numbers
LiquidCrystal lcd(7, 8, 11, 12, 13, 6);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set up LED pins as outputs
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  
  // Set up ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Distance:");
  
  // Turn all LEDs off initially
  digitalWrite(redLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, LOW);
}

void loop() {
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Send a 10 microsecond pulse to trigger pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echo pin and calculate distance in cm
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  
  // Display distance on LCD
  lcd.setCursor(0, 1);
  lcd.print("                "); // Clear line
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print(" cm   ");
  
  // Control LEDs based on distance
  // Turn all LEDs off first
  digitalWrite(redLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, LOW);
  
  // Determine which LED to light based on distance
  if (distance < 10) {
    // Very close - red LED
    digitalWrite(redLed, HIGH);
  } else if (distance >= 10 && distance < 30) {
    // Close - yellow LED
    digitalWrite(yellowLed, HIGH);
  } else if (distance >= 30 && distance < 70) {
    // Medium distance - green LED
    digitalWrite(greenLed, HIGH);
  } else if (distance >= 70) {
    // Far - blue LED
    digitalWrite(blueLed, HIGH);
  }
  
  // Add some delay between readings
  delay(500);
}