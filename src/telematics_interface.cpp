#include "telematics_interface.h"

#define TIME_FRAME_ID 0x039F
#define COMAND_CONTROLLER_ID 0x00FD

constexpr int required_button_counts = 10;
int button_counts = 0;

struct can_frame CAN_A_frame; //Can frame

bool check_start_conditions(MCP2515 CAN_bus, struct Datetime *curr){
  //Serial.println("Checking message");
  if(readMsg(CAN_bus, &CAN_A_frame)){
    //Serial.println(CAN_A_frame.can_id);
    switch(CAN_A_frame.can_id){
      case TIME_FRAME_ID:
        curr->hour = CAN_A_frame.data[0];
        curr->minute = CAN_A_frame.data[1];
        curr->second = CAN_A_frame.data[2];
        curr->day = CAN_A_frame.data[3];
        curr->month = CAN_A_frame.data[4];
        curr->year = CAN_A_frame.data[5];
        return false;
      case COMAND_CONTROLLER_ID:
      //Serial.println(CAN_A_frame.data[0]);
        if(CAN_A_frame.data[0] == 0x20){
          //Serial.println(button_counts);
          button_counts++;
          if(button_counts >= required_button_counts){
            button_counts = 0;
            //Serial.println("Started.");
            digitalWrite(5, HIGH);
            return true;
          }
          else{return false;}
        }
        else{
          button_counts = 0;
          return false;
        }
    }
  }
  else{return false;}
}

bool check_stop_conditions(MCP2515 CAN_bus){
  if(readMsg(CAN_bus, &CAN_A_frame)){
    if(CAN_A_frame.can_id == COMAND_CONTROLLER_ID){
      if(CAN_A_frame.data[0] == 0x20){
        button_counts++;
        if(button_counts >= required_button_counts){
          //Serial.println("Stop.");
          button_counts = 0;
          digitalWrite(5, LOW);
          return true;
        }
        else{return false;}
      }
      else{
        button_counts = 0;
        return false;
      }
    }
    else{return false;}
  }
  else{return false;}
}

void set_filename(struct Datetime *curr, char *name){
  char hour[3];
  char minute[3];
  char second[3];
  char day[3];
  char month[3];
  char year[3];
  sprintf(hour, "%02d", curr->hour);
  sprintf(minute, "%02d", curr->minute);
  sprintf(second, "%02d", curr->second);
  sprintf(day, "%02d", curr->day);
  sprintf(month, "%02d", curr->month);
  sprintf(year, "%02d", curr->year);

  //name = day + month + year;
  //Serial.println(name);

  strcpy(name, day);
  strcat(name, month);
  strcat(name, year);
  strcat(name, "/");
  strcpy(name, hour);
  strcat(name, minute);
  strcat(name, second);
  strcat(name, ".CSV");

  /*for(int i = 0; i < 13; i++){
    Serial.print(name[i]);
  }*/
  //Serial.println(name);
}