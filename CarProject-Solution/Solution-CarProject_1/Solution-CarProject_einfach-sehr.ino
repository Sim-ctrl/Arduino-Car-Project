
//
//  Bitte zuerst in die anderen "kleineren" Projekte schauen! Das Programm hier besteht
//  aus allen 3 Programmen + Ansteuerung zweier Relais
//


#include <SPI.h>      // Bib fuer NFC
#include <MFRC522.h>  // Bib fuer NFC

#include <U8g2lib.h>  //U8g2lib Bib fuer OLED-Display (vglb mit Adafruit) 
#include <Wire.h>     // beim ESP32 wird diese Zeile nicht benötigt

U8G2_SH1106_128X32_VISIONOX_F_HW_I2C oled(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN);      // Instance of the class
MFRC522::MIFARE_Key key;           // Bestandteil fuer NFC Reader 
String tag;   // String wird deklariert
//Um die GPIO3 und GPIO1 Pins nutzen zu können
const int relaisPin1 = 3; // RX-Pin (GPIO 3)
const int relaisPin2 = 1; // TX-Pin (GPIO 1)
boolean aufOderZu = false;  // Boolean um zwischen Offenen Türen und Geschlossenen zu untsch 

void setup() {          // Einmalig ausgeführt (Bei Start des Prg oder betätigen d Reset Btn)
  Serial.begin(9600);   //Fuer den Seriellen Monitor - Debugging
  SPI.begin();          // Init SPI bus
  rfid.PCD_Init();      // Init MFRC522

  oled.begin();           //Methodenaufruf aus der Bib um Display zu starten
  oled.setColorIndex(1);  // Display in Farbmodus setzen ( 1 = schwarz)

  pinMode(relaisPin1, OUTPUT);   //Initialisiere fuer die 2Relais
  pinMode(relaisPin2, OUTPUT);   //Zum öffenen & Schließen 
  digitalWrite(relaisPin1, HIGH); //Dauer-HIGH, damit die Relais keinen strom ziehen
  digitalWrite(relaisPin2, HIGH);
}
void loop() { //wiederholt fortlaufend

  //Um die UID heraus zu bekommen:  
  if ( ! rfid.PICC_IsNewCardPresent())return;//Wenn keine Karte in sich, dann wiederholen
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i]; // UID wird im Strng "tag" gespeichert
    }
    
    Serial.println(tag);
    
    if (tag == "45112194"|| tag =="834110153"){ //Wenn die Tag-UID mit einer unseren UIDS
      oled.clear();                             //übereinstimmt, dann: clear das Bild (OLED)

  
      if(aufOderZu){                            //Wenn Turen geschlossen sind
        digitalWrite(relaisPin1, LOW);          //Mache alle Turen auf
        aufOderZu = !aufOderZu;                 //Mache Variable auf "false" -> Tuer ist auf
        
        oled.clearBuffer();         
        oled.setFont(u8g2_font_logisoso32_tr);  //Schreibe groß "OFFEN"
        oled.drawStr(0,32,"OFFEN");
        oled.sendBuffer();

        
        }else{                               //Ansonsten -> Wenn also Turen gerade auf sind
          digitalWrite(relaisPin2, LOW);     // Mache Turen auf
          aufOderZu = !aufOderZu;            //Mache Variable auf "true" -> Tuer ist zu
          
          oled.clearBuffer();
          oled.setFont(u8g2_font_logisoso32_tr); 
          oled.drawStr(0,32,"Car ZU");            //Schreibe groß "Car ZU"
          oled.sendBuffer();
          
          }
      delay(500);                                   //Pause 500ms
        digitalWrite(relaisPin1, HIGH);             //Schalte wieder aus, weil Tuer schon bewegt
        digitalWrite(relaisPin2, HIGH); // Relais anschalten, sollte daueran bleiben
    } // Ende der if(richtieg UID)
    
    tag = "";  //string wird leer gemacht um zu prüfen ob nochmal was kommt 
    rfid.PICC_HaltA();        //RFID sucht nach neuem Tag
    rfid.PCD_StopCrypto1();  //RFID wird gestoppt
  }
}
