#ifndef Duman_h
#define Duman_h

#include "Arduino.h"

#define DT_VOID 0
#define DT_INT8 1
#define DT_INT16 2
#define DT_INT32 3
#define DT_FLOAT 4
// #define DT_DOUBLE 5
#define TX_BUFFER_SIZE 16
#define RX_BUFFER_SIZE 128

class Duman {
  private:
    Stream& stream;
    uint8_t txBuffer[TX_BUFFER_SIZE];
    uint8_t rxBuffer[RX_BUFFER_SIZE];
    uint8_t rxIdx = 0;
    uint8_t dataLengths[5] = { 0, 1, 2, 4, 4/*, 4*/ }; // void, int8, int16, int32, float, double
    void sendBuffer();
    void fillBuffer(uint8_t messageType, uint8_t dataType, void* dataPtr);
    void fillBufferAndSend(uint8_t messageType, uint8_t dataType, void* dataPtr);

  public:
	Duman(Stream& stream);
    void processMessages();
    void send(uint8_t messageType);
    void send(uint8_t messageType, bool data);
    void send(uint8_t messageType, int8_t data);
    void send(uint8_t messageType, uint8_t data);
    void send(uint8_t messageType, int16_t data);
    void send(uint8_t messageType, uint16_t data);
    void send(uint8_t messageType, int32_t data);
    void send(uint8_t messageType, uint32_t data);
    void send(uint8_t messageType, float data);
    //void send(uint8_t messageType, double data);
    void (*messageProcessor)(uint8_t messageType, void* data, uint8_t dataType);
};


#endif

