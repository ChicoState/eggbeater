#include "Control.h"
#include "usb_interface.h"
#include "fingerprint_reader.h"

#include <stdlib.h>
#include <string.h>

uint32_t control_init(Control_t* c)
{
  if (c == NULL)
    return CtrlError_NullArgument;

  c->CurrentPacket = NULL;
  c->ResponsePacket = NULL;

  // Setup message queues
  return CtrlError_Success;
}

uint32_t control_should_handle_packet(Control_t* c, Packet_t* p)
{
  PacketHeader_t* head;
  uint32_t errCode = 0;

  if (c == NULL || p == NULL)
    return CtrlError_NullArgument;

  if (c->CurrentPacket != NULL)
    return CtrlError_Busy;

  if ((errCode = packet_get_header(p, &head)))
    return errCode;

  switch (head->Command)
  {
    default:
    case CtrlCommand_Bad:
    case CtrlCommand_None:
      errCode = CtrlError_No;
      break;

    case CtrlCommand_OpenSession:
    case CtrlCommand_NewSession:
    case CtrlCommand_RefreshSession:
    case CtrlCommand_CloseSession:
    case CtrlCommand_GetFileKey:
    case CtrlCommand_Cancel:
    case CtrlCommand_KeepAlive:
      errCode = CtrlError_Yes;
      break;
  }

  return errCode;
}

uint32_t control_handle_packet(Control_t* c, Packet_t* p)
{
  uint32_t errCode = 0;
  PacketHeader_t* head;

  if (c == NULL || p == NULL)
    return CtrlError_NullArgument;

  if ((errCode = control_should_handle_packet(c, p)))
    return errCode;

  if ((errCode = packet_get_header(p, &head)))
    return errCode;

  c->CurrentPacket = p;
  c->ResponsePacket = NULL;

  switch (head->Command)
  {
    default:
      errCode = CtrlError_Fail;
      break;

    case CtrlCommand_NewSession:
    case CtrlCommand_OpenSession:
      errCode = control_start_session(c, p);
      break;

    case CtrlCommand_RefreshSession:
      errCode = control_refresh_session(c, p);
      break;

    case CtrlCommand_CloseSession:
      errCode = control_close_session(c, p);
      break;

    case CtrlCommand_GetFileKey:
      errCode = control_get_file_key(c, p);
      break;

    case CtrlCommand_Cancel:
      errCode = control_cancel_current_command(c, p);
      break;

    case CtrlCommand_KeepAlive:
      errCode = control_keep_alive(c, p);
      break;
  }

  return errCode;
}

uint32_t control_is_packet_ready(Control_t* c, Packet_t* p)
{
  if (c == NULL || p == NULL)
    return CtrlError_NullArgument;

  if (c->CurrentPacket != p)
    return CtrlError_Busy;

  // Check fingerprint_task->control_task message queue

  if (c->ResponsePacket != NULL)
    return CtrlError_Yes;
  else
    return CtrlError_No;
}

uint32_t control_get_resp_packet(Control_t* c, Packet_t** d)
{
  if (c == NULL)
    return CtrlError_NullArgument;

  if (c->CurrentPacket == NULL)
    return CtrlError_NotReady;

  if (c->ResponsePacket == NULL)
    return CtrlError_Busy;

  if (d != NULL)
  {
    *d = c->ResponsePacket;
    c->CurrentPacket = NULL;
    c->ResponsePacket = NULL;
  }

  return CtrlError_Success;
}

uint32_t control_start_session(Control_t* c, Packet_t* p)
{
  // Place p on the control_task->fingerprint_task message queue
  return CtrlError_Fail;
}

uint32_t control_refresh_session(Control_t* c, Packet_t* p)
{
  // Place p on the control_task->fingerprint_task message queue
  return CtrlError_Fail;
}

uint32_t control_close_session(Control_t* c, Packet_t* p)
{
  // Place p on the control_task->fingerprint_task message queue
  // Clear CurrentPacket
  // Return success
  return CtrlError_Fail;
}

uint32_t control_get_file_key(Control_t* c, Packet_t* p)
{
  // Place p on the control_task->fingerprint_task message queue
  return CtrlError_Fail;
}

uint32_t control_cancel_current_command(Control_t* c, Packet_t* p)
{
  // Place p on the control_task->fingerprint_task message queue
  return CtrlError_Fail;
}

uint32_t control_keep_alive(Control_t* c, Packet_t* p)
{
  // Construct return keep alive packet
  // Set ResponsePacket
  // Return success
  return CtrlError_Fail;
}
