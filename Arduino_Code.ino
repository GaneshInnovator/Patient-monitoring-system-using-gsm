#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial gsm(10, 11);   // GSM module on digital pins 10 (RX) and 11 (TX)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);   // LCD screen on digital pins 12, 13, 7, 6, 5, 4

// Pulse sensor variables
const int pulsePin = A0;    // Pulse sensor on analog pin A0
int pulseValue = 0;         // Raw pulse sensor value
int pulseBPM = 0;           // Calculated beats per minute (BPM)

// DS18B20 temperature sensor variables
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);    // DS18B20 signal on digital pin 2
DallasTemperature ds18b20(&oneWire);
float tempC;                // Temperature value in degrees Celsius

void setup() {
  // Initialize LCD screen
  lcd.begin(16, 2);
  lcd.print("Initializing...");
  delay(1000);

  // Initialize GSM module
  gsm.begin(9600);
  gsm.println("AT");
  delay(1000);

  // Initialize pulse sensor
  pinMode(pulsePin, INPUT);

  // Initialize temperature sensor
  ds18b20.begin();

  // Display ready message on LCD screen
  lcd.clear();
  lcd.print("Ready");
}

void loop() {
  // Read pulse sensor value and calculate BPM
  pulseValue = analogRead(pulsePin);
  pulseBPM = (int)(60000.0 / pulseValue);

  // Read temperature from DS18B20 sensor
  ds18b20.requestTemperatures();
  tempC = ds18b20.getTempCByIndex(0);

  // Display data on LCD screen
  lcd.clear();
  lcd.print("BPM: ");
  lcd.print(pulseBPM);
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(tempC);
  lcd.print("C");

  // Send data over GSM
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.println("AT+CMGS=\"+xxxxxxxxxxxx\"");   // Replace with phone number to send to
  delay(1000);
  gsm.print("BPM: ");
  gsm.println(pulseBPM);
  gsm.print("Temp: ");
  gsm.print(tempC);
  gsm.print("C");
  delay(1000);
  gsm.write(26);   // Send message
  delay(1000);
}