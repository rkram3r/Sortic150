#ifndef RfidDetector_h
#define RfidDetector_h

#include <MFRC522.h>
#include <Sensor.h>
#include <Config.h>

class RfidDetector : public Sensor
{
public:
  RfidDetector(MFRC522 *rfidReader) : rfidReader{rfidReader} {}

  String get()
  {
    String out{};
    if (cardPresent())
    {
      out += "\"";
      for (auto index = 0; index < RFID_LENGTH; index++)
      {
        out += String(rfidReader->uid.uidByte[index]);
      }
      out += "\"";
    }

    return out;
  }

private:
  MFRC522 *rfidReader;
  bool cardPresent()
  {
    return rfidReader->PICC_IsNewCardPresent() &&
           rfidReader->PICC_ReadCardSerial() &&
           rfidReader->uid.size != 0;
  }
};

#endif