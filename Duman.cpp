
#include "Duman.h"

Duman::Duman(Stream& stream)
	: stream(stream) {
  messageProcessor = NULL;
}

void Duman::send(uint8_t messageType) {
  fillBufferAndSend(messageType, DT_VOID, NULL);
}

void Duman::send(uint8_t messageType, bool data) {
  fillBufferAndSend(messageType, DT_INT8, (void*)&data);
}

void Duman::send(uint8_t messageType, int8_t data) {
  fillBufferAndSend(messageType, DT_INT8, (void*)&data);
}

void Duman::send(uint8_t messageType, uint8_t data) {
  fillBufferAndSend(messageType, DT_INT8, (void*)&data);
}

void Duman::send(uint8_t messageType, int16_t data) {
  fillBufferAndSend(messageType, DT_INT16, (void*)&data);
}

void Duman::send(uint8_t messageType, uint16_t data) {
  fillBufferAndSend(messageType, DT_INT16, (void*)&data);
}

void Duman::send(uint8_t messageType, int32_t data) {
  fillBufferAndSend(messageType, DT_INT32, (void*)&data);
}
void Duman::send(uint8_t messageType, uint32_t data) {
  fillBufferAndSend(messageType, DT_INT32, (void*)&data);
}

void Duman::send(uint8_t messageType, float data) {
  fillBufferAndSend(messageType, DT_FLOAT, (void*)&data);
}

void Duman::fillBuffer(uint8_t messageType, uint8_t dataType, void* dataPtr) {
  uint8_t dataLength = dataLengths[dataType];
  txBuffer[0] = '<';
  txBuffer[1] = dataType;
  txBuffer[2] = messageType;
  byte* ptr = (byte*)dataPtr;
  for (uint8_t i = 0; i < dataLength; i++)
    txBuffer[i + 3] = ptr[i];
  txBuffer[dataLength + 3] = '>';
}

void Duman::fillBufferAndSend(uint8_t messageType, uint8_t dataType, void* dataPtr) {
  fillBuffer(messageType, dataType, dataPtr);
  sendBuffer();
}

void Duman::sendBuffer() {
  uint8_t dataLength = dataLengths[txBuffer[1]];
  for (int i = 0; i < 4 + dataLength; i++)
  	stream.write(txBuffer[i]);
}

void Duman::processMessages() {  
  while (1) {
    if (!stream.available())
      return;
    
    uint8_t data = stream.read();
    if (rxIdx == 0 && data != '<')
      return;
  
    rxBuffer[rxIdx++] = data;
    if (data != '>' || messageProcessor == NULL)
      return;
  
    uint8_t dataType = rxBuffer[1];
    uint16_t messageEnd = dataLengths[dataType] + 3;
    if (rxIdx < messageEnd)
      return;
 
    if (rxBuffer[messageEnd] == '>') {
      uint8_t messageType = rxBuffer[2];
      messageProcessor(messageType, &rxBuffer[3], dataType);
      for (uint8_t i = messageEnd + 1, j = 0; i < rxIdx; i++, j++)
        rxBuffer[j] = rxBuffer[i];
      rxIdx -= messageEnd + 1;
    } else {
      int16_t nextStart = -1;
      for (uint16_t i = 0; i < rxIdx && nextStart == -1; i++)
        if (rxBuffer[i] == '<')
          nextStart = i;
    
      if (nextStart == -1)
        rxIdx = 0;
      else {
        for (uint16_t i = nextStart; i < rxIdx; i++)
          rxBuffer[i - nextStart] = rxBuffer[i];
        rxIdx -= nextStart;
      }
    }
  }
}
