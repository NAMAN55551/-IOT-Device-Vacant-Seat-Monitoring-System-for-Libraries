#define BLYNK_TEMPLATE_ID "TMPL3EJqyI1yj"
#define BLYNK_TEMPLATE_NAME "Air Quality Monitoring"
#define BLYNK_AUTH_TOKEN "U3LW0qy5CN4wBbHB9-UNCwMZCMVozEI4"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte degree_symbol[8] = {
  0b00111,
  0b00101,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "NAMAN-PRAJAPATI 9537"; // Your WiFi name
char pass[] = "4852@2aY";            // Your WiFi password

BlynkTimer timer;

int gas = A0;
int sensorThreshold = 600; // Adjust this threshold as needed

#define DHTPIN 2      // D4 in NodeMCU
#define DHTTYPE DHT11 // Type of DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  int analogSensor = analogRead(gas);
  Blynk.virtualWrite(V2, analogSensor);
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  
  Serial.print("Gas Value: ");
  Serial.println(analogSensor);
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("    Humidity: ");
  Serial.println(h);
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(30000L, sendSensor);

  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.createChar(1, degree_symbol); // Create custom character for degree symbol
  lcd.setCursor(3, 0);
  lcd.print("Air Quality");
  lcd.setCursor(3, 1);
  lcd.print("Monitoring");
  delay(1000);
  lcd.clear();
}

void loop() {
  Blynk.run();
  timer.run();
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasValue = analogRead(gas);
  
  lcd.setCursor(0, 0);
  lcd.print("Temperature ");
  lcd.setCursor(0, 1);
  lcd.print(t);
  lcd.setCursor(6, 1);
  lcd.write(1); // Display degree symbol
  lcd.setCursor(7, 1);
  lcd.print("C");
  delay(5000);
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Humidity ");
  lcd.print(h);
  lcd.print("%");
  delay(5000);
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Gas Value: ");
  lcd.print(gasValue);
  
  if (gasValue < sensorThreshold) {
    lcd.setCursor(0, 1);
    lcd.print("Fresh Air");
    Serial.println("Fresh Air");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Bad Air");
    Serial.println("Bad Air");
    Blynk.logEvent("pollution_alert", "Bad Air"); // Log event for Blynk notification
  }
  
  delay(5000);
  lcd.clear();
}
