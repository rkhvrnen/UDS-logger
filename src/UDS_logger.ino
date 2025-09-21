#include "UDS_handler.h"
#include "UDS_parameters.h"

//Define control unit UDS service CAN-bus ID:s
ECU CR6EU5 = {0x7E0, 0x7E8};

//CAN-bus parameters, baud rates and CAN controller CS pin numbers
#define CAN_E_BITRATE CAN_500KBPS
//#define CAN_A_BITRATE CAN_500KBPS
#define CAN_E_Pin 10
//#define CAN_A_Pin 9

MCP2515 CAN_E(CAN_E_Pin);
//MCP2515 CAN_A(CAN_A_Pin);

uint16_t data_id[] = {0x0163, 0x0165}; //0x0163 length=50(47), 0x0165 length=34(31)
int list_length = 2;
uint8_t separation_time = 2; //Response frame flow control separation time in milliseconds

unsigned long last_print = 0;

const uint32_t can_E_mask = 0xFFFF;// 0b11111111111;
const uint32_t can_E_filter = CR6EU5.UDS_resp_address; 

int req_interval = 125; //UDS request interval

void setup() {
  Serial.begin(115200);
  SPI.begin();
  configCAN(CAN_E, CAN_E_BITRATE, true, can_E_mask, can_E_filter);
}

void loop() {
  run_UDS_routine(CAN_E, &CR6EU5, req_interval, separation_time, list_length, data_id);
}