#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 4); // Change to (0x27,16,2) for 16x2 LCD.
long interval = 60000;
float tempMax;
void setup() {
  myinit();
  bootscreen();
  create_interface();
}
void loop() {
  readValue();
}

void myinit()
{
  tempMax = 0;
  Serial.begin(9600); 
  dht.begin();  
}

void readValue()
{
  float t = dht.readTemperature();
  int h = dht.readHumidity(); 
  float teq =  calculate_humidex(t,h);
  int j;
  lcd.setCursor(2, 0);
  lcd.print(t);
  lcd.setCursor(6, 0);
  lcd.print("C");  
  lcd.setCursor(4, 1);
  lcd.print(h);  
  lcd.setCursor(6, 1);
  lcd.print("%"); 
  lcd.setCursor(11, 0);
  lcd.print(teq); 
  lcd.setCursor(15, 0);
  lcd.print("C");    
  if(t>tempMax)
  {
    tempMax = t;
  }     
  lcd.setCursor(11, 1);
  lcd.print(tempMax); 
  lcd.setCursor(15, 1);
  lcd.print("C");   
  delay(interval);
}
void bootscreen() {
  // set up the LCD's number of columns and rows: 
  lcd.begin();  // lcd 16x2
  lcd.setCursor(0,0); //set cursor to top left corner
  lcd.print("Booting:"); //print the text to the lcd

  for (int i = 0; i <= 100; i++){  // you can change the increment value here
    lcd.setCursor(8,0);
    if (i<100) lcd.print(" ");  //print a space if the percentage is < 100 
    if (i<10) lcd.print(" ");  //print a space if the percentage is < 10
    lcd.print(i);
    lcd.print("%");
    delay(100);  //change the delay to change how fast the boot up screen changes 
  }

  lcd.setCursor(0,0); 
  // Print a message to the LCD.
  lcd.print("STAZIONE METEO");
}

void create_interface() {
  lcd.begin();  // lcd 16x2
  lcd.print("T");
  lcd.setCursor(0,1);
  lcd.print("HUM");
  lcd.setCursor(8,0);
  lcd.print("EQ");
  lcd.setCursor(8,1);
  lcd.print("TM");
  
}


float calculate_humidex(float temperature,float humidity) {
  float e;
  e = (6.112 * pow(10,(7.5 * temperature/(237.7 + temperature))) * humidity/100); //vapor pressure
  float humidex = temperature + 0.55555555 * (e - 10.0); //humidex
  return humidex;
} 
