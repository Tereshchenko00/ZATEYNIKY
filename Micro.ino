#include <SPI.h>
#include <RF24.h>
const uint64_t pipe = "12345";
RF24 radio(9, 10);
int data;
int ans;
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
  // R
  radio.startListening();
  while (!radio.available());
  radio.read(data, sizeof(data));
  Serial.println(data);
  delay(50);
  // T
  radio.stopListening();
  ans = 10;
  radio.write(&ans, sizeof(ans));
  delay(1000);
}
