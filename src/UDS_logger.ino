#include "can_communication.h"

#define debug false

//CAN-bus request and response addresses
#define CR6EU5_req 0x7E0 //UDS request address on CAN-E for CDI control unit
#define CR6EU5_resp 0x7E8 //UDS response address on CAN-E for CDI control unit

//Protocol Control Information (PCI)
#define REQUEST_PCI 0x03
#define RESPONSE_FF_PCI 0x10
#define RESPONSE_CF_PCI 0x20
#define FC_PCI 0x30

//UDS Service IDentifiers (SID)
#define READ_SID 0x22

#define POS_RESP_CODE 0x62

uint16_t data_id[] = {0x0163, 0x0165}; //0x0163 length=50(47), 0x0165 length=34(31)
int list_length = 2;
uint8_t separation_time = 2; //Response frame flow control separation time in milliseconds

unsigned long last_print = 0;

const uint32_t can_mask = 0xFFFF;// 0b11111111111;
const uint32_t can_filter = CR6EU5_resp; 

struct can_frame received; //Can frame
struct can_frame *frame_pointer; //Pointer to the frame structure

unsigned long last_req = 0;
int req_interval = 100; //UDS request interval
int DID_idx = 0;
int UDS_msg_idx = 0;
uint16_t resp_DID = 0;
int last_CF_idx = 0;

//Static allocation for UDS payload
uint8_t UDS_payload[100] = {0};
int UDS_payload_length = 0;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  configCAN(true, can_mask, can_filter);

  //Initialize struct pointer
  frame_pointer = &received;
}

void loop() {
  DID_idx = UDS_read_request(DID_idx);

  if(readMsg(&received)){
    if(debug){
      Serial.print(received.can_id, HEX); Serial.print(":");
      for(int i = 0; i < 8; i++){
        Serial.print(received.data[i], HEX); Serial.print(":");
      }
      Serial.println();
    }
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
          write_flow_ctrl(separation_time);
          return;
        }
        else{break;}
      case RESPONSE_CF_PCI:
        if((received.data[0] & 0x0f) == (last_CF_idx + 1)){
          if(debug){
            Serial.print("CF ID:"); Serial.println((received.data[0] & 0x0f));
          }
          for(int i = 1; i < 8; i++){
            if(debug){Serial.print("IDX: "); Serial.print(UDS_msg_idx); Serial.print(", UDS PL length: "); Serial.print(UDS_payload_length); Serial.println();}
            if(UDS_msg_idx >= (UDS_payload_length - 1)){
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

int UDS_read_request(int idx){
  if(millis() - last_req > req_interval){
    if(debug){
      Serial.print("Request for PID: "); Serial.println(data_id[idx], HEX);
    }
    writeCan(CR6EU5_req, 8, REQUEST_PCI, READ_SID, ((data_id[idx] & 0xFF00) >> 8), (data_id[idx] & 0x00FF), 0, 0, 0, 0, false);
    last_req = millis();
    if(idx >= (list_length - 1)){
      return 0;
    }
    else{return (idx + 1);}
  }
  else{return idx;}
}

void write_flow_ctrl(uint8_t ST){
  writeCan(CR6EU5_req, 8, FC_PCI, 0x08, ST, 0, 0, 0, 0, 0, false);
}

void transmit_message(int length){
  Serial.print(resp_DID, HEX);
  Serial.print(",");
  for(int i = 0; i < length; i++){
    if(UDS_payload[i] < 0x10){Serial.print("0");}
    Serial.print(UDS_payload[i], HEX);
  }
  Serial.println();
}

  /*
  if(readMsg(&received)){
    printFrame(received);
    //Serial.println((received.data[5] << 8) || (received.data[6]));
    
    if(received.data[0] == 0x10){
      //Serial.println("Response arrived!");
      //writeCan(0x7E0, 8, 0x30, 0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, false);
      Serial.println(received.data[5] << 8 | received.data[6]);
    }
  }

  if(millis() - last_transmitted > transmission_interval){
    //writeCan(0x7E0, 8, 0x03, 0x22, 0x01, 0x68, 0x00, 0x00, 0x00, 0x00, false);
    last_transmitted = millis();
    //Serial.println("UDS request sent.");
  }
  */