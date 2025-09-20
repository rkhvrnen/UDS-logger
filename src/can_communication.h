#include "mcp2515.h"

void configCAN(MCP2515 bus, CAN_SPEED bitrate, bool set_filter = false, uint32_t mask = 0, uint32_t filter = 0);
void print(uint8_t X);
void writeCan(MCP2515 bus, uint16_t PID, int DLC, uint8_t data0, uint8_t data1 = 0, uint8_t data2 = 0, uint8_t data3 = 0, uint8_t data4 = 0, uint8_t data5 = 0, uint8_t data6 = 0, uint8_t data7 = 0, bool printing = false);
void printFrame(can_frame frame);
bool readMsg(MCP2515 bus, struct can_frame *received);

const uint32_t MASK = 0b11111111111;