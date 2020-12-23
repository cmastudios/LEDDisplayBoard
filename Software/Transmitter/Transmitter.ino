#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
void loop() {
  int down, up;
  down = !digitalRead(2);
  up = !digitalRead(3);
  if (down || up)
  {
    const uint8_t text[] = {0x01, (down << 1) | up};
    radio.write(&text, sizeof(text));
    delay(100);
  }

}
