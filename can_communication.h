#include "mcp2515.h"

struct CAN_data {
  uint16_t address;
  int dlc;
  uint16_t data;
};

void configCAN(bool set_filter, uint32_t mask, uint32_t filter);
void setCanFilter(uint16_t address[], int length);
void print(uint8_t X);
void writeCan(uint16_t PID, int DLC, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5, uint8_t data6, uint8_t data7, bool printing);
void printFrame(can_frame frame);
bool readMsg(struct can_frame *received);

const uint32_t MASK = 0b11111111111;