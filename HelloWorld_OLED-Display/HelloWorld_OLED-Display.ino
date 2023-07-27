
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SH1106_128X32_VISIONOX_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
//Wählt meinen verwendeten Grafikchips aus (Sh1106)

void setup() {
  u8g2.begin();
}
void loop() {
  u8g2.clearBuffer();
  //bereinigt das Display
  u8g2.setFont(u8g2_font_ncenB08_tr);	
  // wählt schriftart aus 
  //u8g2_font_logisoso32_tr ist bspw 32 PX Hoch (y verändern)
  u8g2.drawStr(0,10,"Hello World!");	
  // Startet bei x=0 y=10 und schreibt "..."
  u8g2.sendBuffer();					
  // überträgt die daten aufs Display
  delay(1000);  
  //Pause 1000ms
}
