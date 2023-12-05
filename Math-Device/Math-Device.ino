/* Mother Theresa Colegio de Iriga Inc. - Mathematical Device 2023 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <NewPing.h>

#define trigPin 11 
#define echoPin 12 
#define servoPin 9 

Servo myservo; // Name for Servo Motor
NewPing sonar(trigPin, echoPin, 50); 
LiquidCrystal_I2C lcd(0x27, 16, 3); 
bool objectDetected = false;
int lastAngle = 0; 
const int maxDistance = 50; 


void setup() {
  Serial.begin(9600);
  lcd.begin();                      // Initialize the LCD
  lcd.backlight();                 // Turn on the backlight
  lcd.setCursor(4, 1);
  lcd.print("MTC de Iriga");       // Print column headers for the boot-up
  lcd.setCursor(5, 2);
  lcd.print("Loading.");
  delay(500);
  lcd.setCursor(5, 2);
  lcd.print("        ");
  lcd.setCursor(5, 2);
  lcd.print("Loading..");
  delay(500);
  lcd.setCursor(5, 2);
  lcd.print("        ");
  lcd.setCursor(5, 2);
  lcd.print("Loading...");         
  myservo.attach(servoPin); 
  delay(700);
}

void loop() {
  int distance = sonar.ping_cm();
  lcd.setCursor(0, 0);
  lcd.print("Angle: "); 
  lcd.setCursor(12, 0);
  lcd.print("Dist: ");
  lcd.setCursor(4, 1);
  lcd.print("              ");
  lcd.setCursor(5, 1);
  lcd.print("X        Y");
  
  
  
  
  if (!objectDetected) {
    lcd.setCursor(0, 2);
    lcd.print(" S e a r c h i n g !"); 
    for (int angle = lastAngle; angle <= 180; angle += 1) {
      myservo.write(angle);
      lcd.setCursor(7, 0);
      lcd.print("     ");
      lcd.setCursor(7, 0);
      lcd.print(angle);
      lcd.setCursor(18, 0);
      lcd.print("  ");
      lcd.setCursor(4, 3);
      lcd.print("           ");
      delay(500);
      
      distance = sonar.ping_cm();
      
      if (angle == 180 && distance == 0) {
        lastAngle = 0; 
        break;
      }
      
      if (distance > 0 && distance < 50) {
        objectDetected = true;
        lastAngle = angle; 
        
        float radian = angle * PI / 180.0;
        float x = distance * cos(radian);
        float y = distance * sin(radian);
      
        lcd.setCursor(7, 0);
        lcd.print("     "); 
        lcd.setCursor(7, 0);
        lcd.print(angle);
        lcd.setCursor(18, 0);
        lcd.print("    "); 
        lcd.setCursor(18, 0);
        lcd.print(distance);
        
        lcd.setCursor(0, 2);
        lcd.print("         "); 
        lcd.setCursor(3, 2);
        lcd.print(x);
        lcd.setCursor(9, 2);
        lcd.print("           "); 
        lcd.setCursor(12, 2);
        lcd.print(y);
        lcd.setCursor(5, 3);
        lcd.print("Detected!!");
        
        delay(100);
        break;
      }
    }
  } else if (objectDetected && (distance == 0 || distance >= maxDistance)) {
    myservo.detach(); 
    
    if (distance == 0 || distance > maxDistance) {
      myservo.attach(servoPin); 
      objectDetected = false;
    }
  }
}
