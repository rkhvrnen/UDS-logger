#include "UDS_handler.h"
#include "UDS_parameters.h"

#define debug false

struct can_frame received; //Can frame

int DID_idx = 0;
int UDS_msg_idx = 0;
int last_CF_idx = 0;
unsigned long last_req = 0;
uint16_t resp_DID = 0;

File datafile;

//Static allocation for UDS payload
uint8_t UDS_payload[100] = {0};
int UDS_payload_length = 0;

void run_UDS_routine(MCP2515 CAN_bus, struct ECU *ecu, int req_interval, uint8_t separation_time, int list_length, uint16_t* id_list){
  DID_idx = UDS_read_request(CAN_bus, ecu, req_interval, DID_idx, list_length, id_list);
  UDS_read(CAN_bus, ecu, separation_time);
}

void UDS_read(MCP2515 CAN_bus, struct ECU *ecu, uint8_t separation_time){
  if(readMsg(CAN_bus, &received)){
    /*if(debug){
      Serial.print(received.can_id, HEX); Serial.print(":");
      for(int i = 0; i < 8; i++){
        Serial.print(received.data[i], HEX); Serial.print(":");
      }
      Serial.println();
    }*/
    switch(received.data[0] & 0xF0){ //Most significant 4 bits define the protocol control information
      case RESPONSE_FF_PCI:
        if(received.data[2] == POS_RESP_CODE){
          resp_DID = (received.data[3] << 8) | received.data[4];
          UDS_msg_idx = 0;
          last_CF_idx = 0;
          UDS_payload_length = (((received.data[0] & 0x0F) << 8) | received.data[1]) - 3;
          for(int i = 5; i < 8; i++){
            *(UDS_payload + UDS_msg_idx) = received.data[i];
            UDS_msg_idx++;
          }
          write_flow_ctrl(CAN_bus, ecu, separation_time);
          return;
        }
        else{break;}
      case RESPONSE_CF_PCI:
        if((received.data[0] & 0x0f) == (last_CF_idx + 1)){
          /*if(debug){
            Serial.print("CF ID:"); Serial.println((received.data[0] & 0x0f));
          }*/
          for(int i = 1; i < 8; i++){
            //if(debug){Serial.print("IDX: "); Serial.print(UDS_msg_idx); Serial.print(", UDS PL length: "); Serial.print(UDS_payload_length); Serial.println();}
            if(UDS_msg_idx >= (UDS_payload_length - 1)){
              *(UDS_payload + UDS_msg_idx) = received.data[i];
              transmit_message(UDS_payload_length);
              break;
            }
            else{
              *(UDS_payload + UDS_msg_idx) = received.data[i];
              UDS_msg_idx++;
            }
          }
          last_CF_idx += 1;
        }
        else{break;}
    }
  }
}

int UDS_read_request(MCP2515 CAN_bus, struct ECU *ecu, int req_interval, int idx, int list_length, uint16_t *id_list){
  if(millis() - last_req > req_interval){
    writeCan(CAN_bus, ecu->UDS_req_address, 8, REQUEST_PCI, READ_SID, ((*(id_list+idx) & 0xFF00) >> 8), (*(id_list+idx) & 0x00FF), 0, 0, 0, 0, false);
    last_req = millis();
    if(idx >= (list_length - 1)){
      return 0;
    }
    else{return (idx + 1);}
  }
  else{return idx;}
}

void write_flow_ctrl(MCP2515 CAN_bus, struct ECU *ecu, uint8_t ST){
  writeCan(CAN_bus, ecu->UDS_req_address, 8, FC_PCI, 0x08, ST, 0, 0, 0, 0, 0, false);
}

void transmit_message(int length){
  datafile = SD.open("data.csv", FILE_WRITE);
  if(datafile){
    datafile.print(millis()); datafile.print(",");
    datafile.print(resp_DID, HEX); datafile.print(",");
    for(int i = 0; i < length; i++){
    if(UDS_payload[i] < 0x10){datafile.print("0");}
      datafile.print(UDS_payload[i], HEX);
    }
    datafile.println();
    datafile.close();
  }else{}
}