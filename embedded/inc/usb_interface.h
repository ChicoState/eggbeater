#ifndef _EGGBEATER_EMBED_USB_INTERFACE_H_
#define _EGGBEATER_EMBED_USB_INTERFACE_H_

#include "usbd_cdc.h"

extern USBD_CDC_ItfTypeDef USB_Interface;

uint8_t USB_Init(USBD_HandleTypeDef);
uint8_t USB_Write(USBD_HandleTypeDef*, uint8_t* buffer, uint32_t length);
uint8_t USB_Read(USBD_HandleTypeDef*, uint8_t* buffer, uint32_t* length);

// User callbacks
void USB_OnReceivePacket(uint8_t* buffer, uint32_t length);

#endif //  _EGGBEATER_EMBED_USB_INTERFACE_H_
