#include <SPI.h>
#include <RF24.h>
const uint64_t pipe = "12345";
RF24 radio(9, 10);
int data;
int ans;
int id;
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(1);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(pipe);
  radio.openReadingPipe(0, pipe);
}

void loop() {
  // S
  while (!Serial.available());
  id = Serial.read();
  if (id != 12345) {
    delay(20);
  } else {
    data = Serial.read();
    // T
    radio.stopListening();
    radio.write(&data, sizeof(data));
    delay(50);
    // R
    radio.startListening();
    while (!radio.available());
    radio.read(ans, sizeof(ans));
    Serial.println(ans);
    delay(1000);
  }

}
