#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
 
#define rx 10
#define resetPin 9
#define greenLed 5
#define redLed 4
#define relay 3
#define buzzer 2
#define accessDelay 2000
#define deniedDelay 1000
MFRC522 mfrc522(rx, resetPin);
 
void setup() 
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  noTone(buzzer);
  digitalWrite(relay, LOW);
  Serial.println("Place access control card on reader...");
  Serial.println();

}
void loop() 
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  Serial.print("Tag UID :");
  String tagUid= "";
  byte hexByte;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     tagUid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     tagUid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  tagUid.toUpperCase();
  if (tagUid.substring(1) == "C6 8A 99 F8")
  {
    Serial.println("Access Granted");
    Serial.println();
    delay(1000);
    digitalWrite(relay, HIGH);
    digitalWrite(greenLed, HIGH);
    delay(accessDelay);
    digitalWrite(relay, LOW);
    digitalWrite(greenLed, LOW);
  }
 
 else   {
    Serial.println("Access denied");
    digitalWrite(redLed, HIGH);
    tone(buzzer, 300);
    delay(deniedDelay);
    digitalWrite(redLed, LOW);
    noTone(buzzer);
  }
}