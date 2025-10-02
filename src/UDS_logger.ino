#include "UDS_handler.h"
#include "UDS_parameters.h"
#include "telematics_interface.h"

//Define control unit UDS service CAN-bus ID:s
ECU CR6EU5 = {0x7E0, 0x7E8};

//CAN-bus parameters, baud rates and CAN controller CS pin numbers
#define CAN_E_BITRATE CAN_500KBPS
#define CAN_A_BITRATE CAN_125KBPS
#define CAN_E_Pin 10
#define CAN_A_Pin 9

MCP2515 CAN_E(CAN_E_Pin);
MCP2515 CAN_A(CAN_A_Pin);

bool logging_active;

Datetime curr_time = {0, 0, 0, 0, 0, 0};

constexpr uint16_t data_id[] = {0x0163, 0x0165}; //0x0163 length=50(47), 0x0165 length=34(31)
constexpr int list_length = 2;
constexpr uint8_t separation_time = 2; //Response frame flow control separation time in milliseconds

constexpr uint32_t can_E_mask = 0xFFFF;// 0b11111111111;
const uint32_t can_E_filter = CR6EU5.UDS_resp_address; 

const int req_interval = 125; //UDS request interval

//File datafile;
char filename[23];

void setup() {
  //Initialize CAN bus
  //Serial.begin(115200);
  SPI.begin();
  configCAN(CAN_E, CAN_E_BITRATE, false, true, can_E_mask, can_E_filter);
  configCAN(CAN_A, CAN_A_BITRATE, true); //Set filters?

  SD.begin(4);
  
  //UDS_data = SD.open("UDS_log.csv", FILE_WRITE);
  logging_active = false;

  //Mute button simulation
  //writeCan(CAN_A, 0x03F6, 5, 0x02, 0x40, 0, 0, 0);

}

void loop() {
  //writeCan(CAN_A, 0x01EE, 3, 0x01, 0, 0xC3);
  //Serial.println(logging_active);
  if(logging_active){
    //Serial.println("Active");
    run_UDS_routine(CAN_E, &CR6EU5, req_interval, separation_time, list_length, data_id, filename);
    if(check_stop_conditions(CAN_A) == true){
      logging_active = false;
    }
  }
  else{
    //Serial.println("Running loop");
    //read_time(&curr_time); //NOT NEEDED; INCLUDED IN START CONDITIONS?
    if(check_start_conditions(CAN_A, &curr_time) == true){
      logging_active = true; //Pass filename?
      //memset(&filename, 0, 14);
      set_filename(&curr_time, filename);
      //writeCan(CAN_A, 0x01EE, 3, 0xFF, 0, 0xC3); //64, 0, C3
    }
  }
}