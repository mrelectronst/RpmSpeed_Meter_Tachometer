#include <LiquidCrystal.h>
LiquidCrystal lcd(6, 7, 2, 3, 4, 5);  // LCD Pins

const float circumference = 149.15; // Write Wheel Circumference *cm -2*pi*r-

uint8_t hall_Thresh = 10; // "10 value +25rpm" set number of hall trips for RPM reading (higher improves accuracy)

uint8_t hall_Count; // Counter for each spin

// Calculation Speed
void calc_Speed(float passed_Time)
{

  // Calculation Wheel Speed 
  float m_Speed = (circumference / 100000) * hall_Count / (passed_Time / 3600);
  lcd.setCursor(8, 1);
  lcd.print((int)m_Speed);
  lcd.setCursor(11, 1);
  lcd.print("km/h");
}

// Calculation RPM
void calc_Rpm(float passed_Time)
{
  // Calculation RPM 
  float rpm_Val =(hall_Count/passed_Time) * 60;
  lcd.setCursor(0, 1);
  lcd.print((int)rpm_Val);
  lcd.setCursor(3, 1);
  lcd.print("rpm");
}

void clear_LCD_RpmSpeed(float start_Time)
{
    // set 0 values on lcd screen     
    float e_Time = millis();
    if((e_Time - start_Time) / 1000 > 12)
    {
      lcd.setCursor(0, 1);
      lcd.print("0  rpm");
      lcd.setCursor(8, 1);
      lcd.print("0  km/h");  
    }
}

void setup() {
  pinMode(8, INPUT);

  //Initialize LCD 
  lcd.begin(16, 2);
  lcd.print("RPM");
  lcd.setCursor(0, 1);
  lcd.print("0  rpm");

  lcd.setCursor(8, 0);
  lcd.print("SPEED");
  lcd.setCursor(8, 1);
  lcd.print("0  km/h");

}

void loop() {
  
  // Initialize values
  hall_Count = 1;
  float start = millis();
  bool on_state = false;

  // Calculation Spin Time
  while (1) {
    if (digitalRead(8) == 0) 
    {
      if (on_state == false) 
      {
        on_state = true;
        hall_Count++;                   // Increase counter in each spin 
      }
    }
    else 
    {
      on_state = false;
    }

    clear_LCD_RpmSpeed(start);          // Control whether movement

    if (hall_Count >= hall_Thresh)      // Control counter overflow as threshold 
    {
      break;
    }
  } 

  // Calculation Passed Time 
  float end_Time = millis();
  float time_Passed = (end_Time - start) / 1000;

  calc_Speed(time_Passed);
  calc_Rpm(time_Passed);

  delay(1);// delay in between reads for stability
}
