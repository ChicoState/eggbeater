#ifndef _EGGBEATER_CONTROL_H_
#define _EGGBEATER_CONTROL_H_

#include "common.h"
#include "Packet.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Control_t
{
  Packet_t CurrentPacket;
  Packet_t ResponsePacket;
} Control_t;

typedef enum CtrlError_t
{
  CtrlError_Success       = 0x0000,
  CtrlError_Yes           = 0x0000,
  CtrlError_No            = 0x0001,
  CtrlError_Fail          = 0x0001,
  CtrlError_NullArgument  = 0x8001,
  CtrlError_Busy          = 0x8002,
  CtrlError_NotReady      = 0x8003
} CtrlError_t;

// Fingerprint task to control task message queues
RTOS_Data_Queue_t ctrlDataQueue;
Control_t         ctrlState;

//! @section Control flow functions
//! Initialize the control subsystem
uint32_t control_init(Control_t*);
//! Check if the control system should handle the specified packet
uint32_t control_should_handle_packet(Control_t*, Packet_t*);
//! Schedule handling of the specified packet, if required
uint32_t control_handle_packet(Control_t*, Packet_t*);
//! Check if a response packet is ready for the specified packet
//! @note Not all packets have a response
uint32_t control_is_packet_ready(Control_t*, Packet_t*);
//! Get the response packet for the last handled packet
//! @note This function will clear the current and response packet pointers
uint32_t control_get_resp_packet(Control_t*, Packet_t*);

//! @section Packet handler functions
uint32_t control_start_session(Control_t*, Packet_t*);
uint32_t control_refresh_session(Control_t*, Packet_t*);
uint32_t control_close_session(Control_t*, Packet_t*);
uint32_t control_get_file_key(Control_t*, Packet_t*);
uint32_t control_cancel_current_command(Control_t*, Packet_t*);
uint32_t control_keep_alive(Control_t*, Packet_t*);

#ifdef __cplusplus
};
#endif

#endif // _EGGBEATER_CONTROL_H_
