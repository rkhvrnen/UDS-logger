#include "can_communication.h"

//Protocol Control Information (PCI)
#define REQUEST_PCI 0x03
#define RESPONSE_FF_PCI 0x10
#define RESPONSE_CF_PCI 0x20
#define FC_PCI 0x30

//UDS Service IDentifiers (SID)
#define READ_SID 0x22

#define POS_RESP_CODE 0x62

void run_UDS_routine(MCP2515 CAN_bus, struct ECU *ecu, int req_interval, uint8_t separation_time, int list_length, uint16_t *data_id);
void UDS_read(MCP2515 CAN_bus, struct ECU *ecu, uint8_t separation_time);
int UDS_read_request(MCP2515 CAN_bus, struct ECU *ecu, int req_interval, int idx, int list_length, uint16_t* id_list);
void write_flow_ctrl(MCP2515 CAN_bus, struct ECU *ecu, uint8_t ST);
void transmit_message(int length);