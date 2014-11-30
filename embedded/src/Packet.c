#include "Packet.h"

#include <stdlib.h>
#include <string.h>

#define EGGBEATER_SOF_BYTE  0x55
#define EGGBEATER_EOF_BYTE  0xff

#define COMMAND_VALID(cmd)  ( ( ( cmd ) == CtrlCommand_None ) \
                           || ( ( cmd ) == CtrlCommand_Echo ) \
                           || ( ( cmd ) == CtrlCommand_OpenSession ) \
                           || ( ( cmd ) == CtrlCommand_NewSession ) \
                           || ( ( cmd ) == CtrlCommand_RefreshSession ) \
                           || ( ( cmd ) == CtrlCommand_CloseSession ) \
                           || ( ( cmd ) == CtrlCommand_GetFileKey ) \
                            )

uint32_t packet_validate(Packet_t* p)
{
  PacketHeader_t* header = NULL;
  uint16_t specLen, calcLen;

  if (p == NULL)
    return PacketError_NullArgument;

  if (p->Data == NULL)
    return PacketError_NullArgument;

  if (p->Length < sizeof(PacketHeader_t))
    return PacketError_InvalidPacket;

  header = (PacketHeader_t*)p->Data;

  if (header->MagicNumber != EGGBEATER_SOF_BYTE)
    return PacketError_BadSOF;

  if (!COMMAND_VALID(header->Command))
    return PacketError_BadCommand;

  specLen = (header->LengthHigh << 8) | (header->LengthLow);
  calcLen = specLen + sizeof(PacketHeader_t);

  if (calcLen != p->Length)
    return PacketError_BadSize;

  if (p->Data[calcLen - 1] != EGGBEATER_EOF_BYTE)
    return PacketError_BadEOF;

  return 0;
}

uint32_t packet_get_header(Packet_t* p, PacketHeader_t** h)
{
  if (p == NULL)
    return PacketError_NullArgument;

  if (p->Data == NULL)
    return PacketError_NullArgument;

  if (h != NULL)
    *h = (PacketHeader_t*)p->Data;

  return 0;
}

uint32_t packet_get_data(Packet_t* p, uint8_t** d)
{
  if (p == NULL)
    return PacketError_NullArgument;

  if (p->Data == NULL)
    return PacketError_NullArgument;

  if (d != NULL)
    *d = &(((PacketHeader_t*)(p->Data))->Data0);

  return 0;
}

uint32_t packet_get_data_length(Packet_t* p, uint16_t* l)
{
  PacketHeader_t* header;

  if (p == NULL)
    return PacketError_NullArgument;

  if (p->Data == NULL)
    return PacketError_NullArgument;

  header = (PacketHeader_t*)p->Data;

  if (l != NULL)
    *l = (header->LengthHigh << 8) | (header->LengthLow);

  return 0;
}

uint32_t packet_create(Packet_t* p, uint8_t cmd, uint8_t* data, uint32_t dataLen)
{
  PacketHeader_t* head;
  uint8_t* pData;
  uint32_t errCode;

  if (p == NULL)
    return PacketError_NullArgument;

  if (data == NULL && dataLen != 0)
    return PacketError_NullArgument;

  if (p->Data != NULL)
  {
    free(p->Data);
    p->Data = NULL;
    p->Length = 0;
  }

  p->Length = sizeof(PacketHeader_t) + dataLen;
  p->Data = malloc(p->Length);

  if ((errCode = packet_get_header(p, &head)) != 0)
    return errCode;

  head->MagicNumber = EGGBEATER_SOF_BYTE;
  head->Command     = cmd;
  head->LengthHigh  = (dataLen >> 8) & 0xff;
  head->LengthLow   = (dataLen) & 0xff;

  if ((errCode = packet_get_data(p, &pData)) != 0)
    return errCode;

  if (dataLen > 0)
    memcpy(pData, data, dataLen);

  p->Data[p->Length - 1] = EGGBEATER_EOF_BYTE;

  return 0;
}

uint32_t packet_free(Packet_t* p)
{
  if (p == NULL)
    return PacketError_NullArgument;

  if (p->Data != NULL)
  {
    free(p->Data);
    p->Data = NULL;
    p->Length = 0;
  }
  
  return 0;
}
