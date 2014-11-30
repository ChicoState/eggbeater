#ifndef _EGGBEATER_PACKET_H_
#define _EGGBEATER_PACKET_H_

#include "Common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum CtrlCommand_t
{
  CtrlCommand_Bad             = 255,
  CtrlCommand_None            = 0,
  CtrlCommand_Echo            = 1,
  CtrlCommand_OpenSession     = 4,
  CtrlCommand_NewSession      = 5,
  CtrlCommand_RefreshSession  = 6,
  CtrlCommand_CloseSession    = 7,
  CtrlCommand_GetFileKey      = 9,
  CtrlCommand_Cancel          = 10,
  CtrlCommand_KeepAlive       = 11,
  CtrlCommand_Busy            = 12
} CtrlCommand_t;

typedef struct PacketHeader_t
{
  uint8_t     MagicNumber,
              Command,
              LengthHigh,
              LengthLow,
              Data0;
} PacketHeader_t;

/*
typedef struct Packet_t
{
  uint8_t*  Data;
  uint32_t  Length;
} Packet_t; // */

typedef enum PacketError_t
{
  PacketError_NullArgument  = 0x8001,
  PacketError_InvalidPacket = 0x8002,
  PacketError_BadSOF        = 0x8003,
  PacketError_BadCommand    = 0x8004,
  PacketError_BadSize       = 0x8005,
  PacketError_BadEOF        = 0x8006
} PacketError_t;

//! @section Packet functions
//! Check that the specified packet is valid
uint32_t packet_validate(Packet_t*);
//! Get the packet header of the specified packet
//! @note The packet does not need to be valid. The user should be aware of
//! @note this condition.
uint32_t packet_get_header(Packet_t*, PacketHeader_t**);
//! Get a pointer to the data section of the specified packet
uint32_t packet_get_data(Packet_t*, uint8_t**);
//! Get the length of the data section of the specified packet
uint32_t packet_get_data_length(Packet_t*, uint16_t*);
//! Create a packet from a command, a data section and a length
uint32_t packet_create(Packet_t*, uint8_t, uint8_t*, uint32_t);
//! Free memory associated the the specified packet
uint32_t packet_free(Packet_t*);

#ifdef __cplusplus
};
#endif

#endif // _EGGBEATER_PACKET_H_
