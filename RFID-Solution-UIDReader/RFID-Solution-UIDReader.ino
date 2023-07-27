#include <SPI.h>        // Bib fuer NFC
#include <MFRC522.h>    // Bib fuer NFC
constexpr uint8_t RST_PIN = D3; //D3 fuer Reset des RFID Reader    
constexpr uint8_t SS_PIN = D4;  // D4 fuer SDA am RFID Reader 
MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class
MFRC522::MIFARE_Key key;        // Bestandteil fuer RFID Reader  
String tag;                     // String wird deklariert

//Ich verwende ein ESP8266 und ein RFID-RC522 Modul

//Wiefolgt habe ich den RFID-Reader angeschlossen:
//SDA - D4
//SCK - D5
//MOSI - D7
//MISO - D6
//IRQ - Schwebend
//GND - GND
//RST - D3
//3,3V - 3v3


void setup() {          // Einmalig ausgeführt (Bei Start des Prg oder betätigen d Reset Btn)
  
  Serial.begin(9600);   // Um den seriellen Monitor zu verwenden  
  SPI.begin();          // Init SPI bus (NFC Lesegerät)
  rfid.PCD_Init();      // Init MFRC522 (NFC Lesegerät)
}


void loop() { //Wird fortlaufent durchgeführt

  
  if ( ! rfid.PICC_IsNewCardPresent()) // Er wiederholt, wenn keine NFC Spule erkannt wurde
    return;                            // Der Rest des Programms wird nicht ausgeführt, er prüft so lange bis NFC einmal präsent war
//Dieser Teil wird nur erreicht, wenn irgendeine NFC Karte anlag
  if (rfid.PICC_ReadCardSerial()) {     // Wenn etwas zu lesen ist 
    for (byte i = 0; i < 4; i++) {      // 4 Wiederholungen, da die UIDs von 00 00 00 00 bis 99 99 99 99 gehen 
      tag += rfid.uid.uidByte[i];       //String wird initialisiert und nimmt die UID des NFC Chips an  
    }

    Serial.println(tag);        // In die Konsole Schreiben; fur Debugging 
    tag = "";                   //String wieder leeren für nächsten Prüfvorgang
    rfid.PICC_HaltA();          //Hält das System kurz an (NFC Reader)
    rfid.PCD_StopCrypto1();     //Stoppt den nfc reader kurz
    delay(500);                 //Pause
  }


}
