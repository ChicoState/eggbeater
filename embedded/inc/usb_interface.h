#ifndef _EGGBEATER_EMBED_USB_INTERFACE_H_
#define _EGGBEATER_EMBED_USB_INTERFACE_H_

#include <usbd_core.h>
#include <usbd_cdc.h>
#include <usbd_desc.h>

#include "common.h"

extern USBD_CDC_ItfTypeDef  USB_Interface;
extern USBD_HandleTypeDef   USBD_Device;

typedef struct USB_Packet
{
  uint8_t* Data;
  uint32_t Length;
} USB_Packet;

typedef struct USB_Write_Data
{
  QueueHandle_t TransmitQueue;
  QueueHandle_t ReceiveQueue;
} USB_Write_Data;

USB_Write_Data usbWriteData;

void InitUSB(void);
void USB_ReadyToReceive(void);

// User callbacks
void USB_OnReceivePacket(uint8_t* buffer, uint32_t length);

// RTOS task entry point
void USB_Write_Task(void*);

#endif //  _EGGBEATER_EMBED_USB_INTERFACE_H_
