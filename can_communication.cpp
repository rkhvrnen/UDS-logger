#include "can_communication.h"

#define CAN_BITRATE CAN_500KBPS
#define CAN_Pin 10

MCP2515 mcp2515(CAN_Pin);

//char filter_registers[] = {RXF0, RXF1, RXF2, RXF3, RXF4, RXF5}

void configCAN(bool set_filter, uint32_t mask, uint32_t filter){
  mcp2515.reset();
  mcp2515.setBitrate(CAN_BITRATE);
  //mcp2515.setConfigMode();
  //mcp2515.setLoopbackMode();

  if(set_filter){
    mcp2515.setConfigMode();
    mcp2515.setFilterMask(MCP2515::MASK0, 0, mask);
    mcp2515.setFilterMask(MCP2515::MASK1, 0, mask);
    mcp2515.setFilter(MCP2515::RXF(0), 0, filter);
  }

  mcp2515.setNormalMode();
}


void setCanFilter(uint16_t address[], int length){
  Serial.print("Address array length: "); Serial.println(length);
    mcp2515.setFilterMask(MCP2515::MASK0, 0, MASK); mcp2515.setFilterMask(MCP2515::MASK1, 0, MASK);
    for(int i = 0; i < length; i++){
      mcp2515.setFilter(MCP2515::RXF(i), 0, address[i]);
    }
    mcp2515.setNormalMode();
}

void writeCan(uint16_t PID, int DLC, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5, uint8_t data6, uint8_t data7, bool printing){
  struct can_frame frame;
  frame.can_id = PID;
  frame.can_dlc = DLC;
  frame.data[0] = data0;
  frame.data[1] = data1;
  frame.data[2] = data2;
  frame.data[3] = data3;
  frame.data[4] = data4;
  frame.data[5] = data5;
  frame.data[6] = data6;
  frame.data[7] = data7;
  mcp2515.sendMessage(&frame);
  if(printing){
    Serial.print("ID: "); Serial.print(PID, HEX);
    Serial.print(" Data: ");
    print(frame.data[0]); Serial.print(":");
    print(frame.data[1]); Serial.print(":");
    print(frame.data[2]); Serial.print(":");
    print(frame.data[3]); Serial.print(":");
    print(frame.data[4]); Serial.print(":"); 
    print(frame.data[5]); Serial.print(":");
    print(frame.data[6]); Serial.print(":");
    print(frame.data[7]); Serial.println();
  }
}

void print(uint8_t X) {
   if (X < 16) {Serial.print("0");}
   Serial.print(X, HEX);
}

void printFrame(can_frame frame){
  Serial.print("FRAME ID="); Serial.print(frame.can_id, HEX); Serial.print("; DLC="); Serial.print(frame.can_dlc); Serial.print("; Data=");
  for (int i = 0; i < frame.can_dlc; i++){
    char buffer[5];
    Serial.print(":");
    sprintf(buffer,"%02X", frame.data[i]);
    Serial.print(buffer);
  }
  Serial.println();
}

bool readMsg(struct can_frame *received){
  if(mcp2515.readMessage(received) == MCP2515::ERROR_OK){
    return 1;
  }
  else{return 0;}
}