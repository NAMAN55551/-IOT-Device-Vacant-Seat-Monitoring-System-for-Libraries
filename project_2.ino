#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define trigPin  D5  // Adjust pin numbers according to your connections
#define echoPin  D6
#define btrigPin D3
#define bechoPin D4
#define vtrigPin D7
#define vechoPin D8

int s = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2); 
void setup()
{
  Serial.begin(9600);

  lcd.backlight();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(btrigPin, OUTPUT);
  pinMode(bechoPin, INPUT);
  pinMode(vtrigPin, OUTPUT);
  pinMode(vechoPin, INPUT);

  lcd.print("Library vacant seat in");
}

void loop()
{
  int duration, distance;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  int bduration, bdistance;
  digitalWrite(btrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(btrigPin, LOW);
  bduration = pulseIn(bechoPin, HIGH);
  bdistance = (bduration/2) / 29.1;

  int vduration, vdistance;
  digitalWrite(vtrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(vtrigPin, LOW);
  vduration = pulseIn(vechoPin, HIGH);
  vdistance = (vduration/2) / 29.1;

  Serial.println(distance);
  Serial.println(bdistance);
  Serial.println(vdistance);

  if (distance >= 10)
  {
    s = 1;
    if (bdistance >= 10 || vdistance >= 10)
    {
      s = 2;
      if (vdistance >= 10 && bdistance >= 10)
      {
        s = 3;
      }
    }
  }
  else if (bdistance <= 10 && distance <= 10 && vdistance <= 10)
  {
    s = 0;
  }
  else if (bdistance >= 10 && vdistance >= 10)
  {
    s = 2;
  }
  else if (bdistance >= 10 || vdistance >= 10)
  {
    s = 1;
  }

  lcd.setCursor(0, 1);
  lcd.print(s);
  lcd.print(" out of 3");
  lcd.setBacklight(1);
  delay(500); // Wait for 500 milliseconds
}
