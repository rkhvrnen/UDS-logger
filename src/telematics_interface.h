#include "can_communication.h"

typedef struct Datetime{
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint8_t day;
  uint8_t month;
  uint8_t year;
};

bool check_start_conditions(MCP2515 CAN_bus, struct Datetime *curr);
bool check_stop_conditions(MCP2515 CAN_bus);
void set_filename(struct Datetime *curr, char *name);