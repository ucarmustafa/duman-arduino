#include <SoftwareSerial.h>
#include <Duman.h>

Duman duman(Serial); // use Serial for duman communication
SoftwareSerial ss(8, 9);

void processMessage(uint8_t messageType, void* data, uint8_t dataType);

void setup() {
  // set received message processor:
  duman.messageProcessor = processMessage;

  // init Serial
  Serial.begin(115200);
  Serial.setTimeout(10);

  // init Software serial
  ss.begin(115200);
  ss.setTimeout(10);

  // data to be sent
  bool booleanData = true;
  int8_t signedByteData = -10;
  uint8_t unsignedByteData = 200;

  /* sending demo */
  duman.send(0);
  duman.send(1, true);
  duman.send(2, false);
  duman.send(3, -5);
  duman.send(4, 200);
  duman.send(5, 1000);
  duman.send(5, -1000);
  duman.send(6, 2000);
  duman.send(7, 1000000);
  duman.send(8, -1000000);
  duman.send(9, 1.23f);
  duman.send(10, 23.27f);
  duman.send(11, booleanData);
  duman.send(12, signedByteData);
  duman.send(13, unsignedByteData);
}

void loop() {
  // checks messages and invokes processMessage method if a new message received
  duman.processMessages();
}

void processMessage(uint8_t messageType, void* data, uint8_t dataType) {
  ss.print(messageType); // print received message type
  ss.print(' ');
  ss.print(dataType); // print data type (uint8_t, uint16_t etc.)
  ss.print(' ');

  if (dataType == DT_VOID) // if no data available (like duman.send(0)), there is nothing else to print
    ss.println();
  else if (dataType == DT_INT8)  // if data type is int8_t
    ss.println(*(int8_t*)data);  //   convert void* to uint8_t*, then get value from pointer
  else if (dataType == DT_INT16) // if data type is int16_t
    ss.println(*(int16_t*)data); //   convert void* to uint16_t*, then get value from pointer
  else if (dataType == DT_INT32) // if data type is int32_t
    ss.println(*(int32_t*)data); //   convert void* to uint32_t*, then get value from pointer
  else if (dataType == DT_FLOAT) // if data type is float
    ss.println(*(float*)data);   //   convert void* to float*, then get value from pointer
}

