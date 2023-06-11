#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "Rina1";
void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setChannel(21);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
enum
{
  RX_ST_IDLE,
  RX_ST_RX
} rxSt = RX_ST_IDLE;
uint32_t stateChangeTimestamp = 0;
void loop() {
  int down, up;
  down = !digitalRead(2);
  up = !digitalRead(3);
  if (down || up)
  {
    const uint8_t text[] = {0x01, (down << 1) | up, 'K', 'N', '6', 'V', 'J', 'P'};
    radio.write(&text, sizeof(text));
    delay(250);
  }

  if (Serial.available() > 0)
  {
    Serial.setTimeout(50);
    String cmd = Serial.readStringUntil('\r');
    if (cmd.length() > 2 && cmd[0] == 'T' && cmd[1] == 'X')
    {
      String txhex = cmd.substring(2);
      if (txhex.length() % 2 != 0 || txhex.length() > 64)
      {
        Serial.println("E01");
      }
      else
      {
        int len = txhex.length() / 2;
        uint8_t txbuf[32];
        for (int i = 0; i < len; ++i)
        {
          txbuf[i] = strtoul(txhex.substring(i*2, i*2+2).c_str(), NULL, 16);
        }
        if (radio.write(txbuf, len))
        {
          Serial.println("ACK");
        }
        else
        {
          Serial.println("NACK");
        }
      }
    }
  }



}
