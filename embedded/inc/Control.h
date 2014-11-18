#ifndef _EGGBEATER_CONTROL_H_
#define _EGGBEATER_CONTROL_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum CtrlCommand_t
{
  CtrlCommand_Bad             = -1,
  CtrlCommand_None            = 0,
  CtrlCommand_Echo            = 1,
  CtrlCommand_OpenSession     = 4,
  CtrlCommand_NewSession      = 5,
  CtrlCommand_RefreshSession  = 6,
  CtrlCommand_CloseSession    = 7,
  CtrlCommand_GetFileKey      = 9
} CtrlCommand_t;

typedef struct PacketHeader_t
{
  uint8_t     MagicNumber,
              Command,
              LengthHigh,
              LengthLow,
              Data0;
} PacketHeader_t;

typedef struct Packet_t
{
  uint8_t*  Data;
  uint32_t  Length;
} Packet_t;

typedef enum PacketError_t
{
  PacketError_NullArgument = 0x8001,
  PacketError_InvalidPacket = 0x8002
} PacketError_t;

uint32_t packet_validate(Packet_t*);

uint32_t packet_get_header(Packet_t*, PacketHeader_t**);

uint32_t packet_get_data(Packet_t*, uint8_t**);

uint32_t packet_get_data_length(Packet_t*, uint16_t*);

uint32_t packet_create(Packet_t*, uint8_t, uint8_t*, uint32_t);

//! @todo Yep

#ifdef __cplusplus
};
#endif

#endif // _EGGBEATER_CONTROL_H_
