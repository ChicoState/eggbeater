#ifndef _EGGBEATER_EMBED_USB_INTERFACE_H_
#define _EGGBEATER_EMBED_USB_INTERFACE_H_

#include <usbd_core.h>
#include <usbd_cdc.h>
#include <usbd_desc.h>

#include "common.h"

extern USBD_CDC_ItfTypeDef  USB_Interface;
extern USBD_HandleTypeDef   USBD_Device;

#ifdef __cplusplus
extern "C" {
#endif

RTOS_Data_Queue_t usbWriteData;

void InitUSB(void);
void USB_ReadyToReceive(void);

// User callbacks
// Called when a packet is received via USB
// The buffer should be copied from, as its contents will change
void USB_OnReceivePacket(uint8_t* buffer, uint32_t length);

// RTOS task entry point
void USB_Write_Task(void*);

#ifdef __cplusplus
};
#endif

#endif //  _EGGBEATER_EMBED_USB_INTERFACE_H_
