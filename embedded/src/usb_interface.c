#include <stdint.h>

#include "usb_interface.h"
#include "packet.h"

#include <stm32f429i_discovery.h>

int8_t USB_Interface_Init(void);
int8_t USB_Interface_Close(void);
int8_t USB_Interface_Control(uint8_t cmd, uint8_t* buffer, uint16_t length);
int8_t USB_Interface_Read(uint8_t* buffer, uint32_t *length);

USBD_HandleTypeDef USBD_Device;
uint32_t OTG_ShouldYield = 0;

extern PCD_HandleTypeDef  hpcd;

USBD_CDC_ItfTypeDef USB_Interface = {
  .Init = USB_Interface_Init,
  .DeInit = USB_Interface_Close,
  .Control = USB_Interface_Control,
  .Receive = USB_Interface_Read
};

USBD_CDC_LineCodingTypeDef linecoding =
{
  115200, /* baud rate*/
  0x00,   /* stop bits-1*/
  0x00,   /* parity - none*/
  0x08    /* nb. of bits 8*/
};

typedef struct
{
  uint8_t   Buffer[CDC_DATA_FS_MAX_PACKET_SIZE];
  uint32_t  Length;
} USB_RxBuffer;

USB_RxBuffer ReceiveBuffer = {{0}, 0};

void OTG_HS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd);

  portYIELD_FROM_ISR(OTG_ShouldYield);
}

void OTG_FS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd);

  portYIELD_FROM_ISR(OTG_ShouldYield);
}

int8_t USB_Interface_Init(void)
{
  // Set the Rx buffer
  USBD_CDC_SetRxBuffer(&USBD_Device, ReceiveBuffer.Buffer);

  return 0;
}

int8_t USB_Interface_Close(void)
{
  return 0;
}

int8_t USB_Interface_Control(uint8_t cmd, uint8_t* buffer, uint16_t length)
{
  switch (cmd)
  {
  case CDC_SEND_ENCAPSULATED_COMMAND:
    /* Add your code here */
    break;

  case CDC_GET_ENCAPSULATED_RESPONSE:
    /* Add your code here */
    break;

  case CDC_SET_COMM_FEATURE:
    /* Add your code here */
    break;

  case CDC_GET_COMM_FEATURE:
    /* Add your code here */
    break;

  case CDC_CLEAR_COMM_FEATURE:
    /* Add your code here */
    break;

  case CDC_SET_LINE_CODING:
    linecoding.bitrate    = (uint32_t)(buffer[0] | (buffer[1] << 8) |\
                            (buffer[2] << 16) | (buffer[3] << 24));
    linecoding.format     = buffer[4];
    linecoding.paritytype = buffer[5];
    linecoding.datatype   = buffer[6];

    /* Add your code here */
    break;

  case CDC_GET_LINE_CODING:
    buffer[0] = (uint8_t)(linecoding.bitrate);
    buffer[1] = (uint8_t)(linecoding.bitrate >> 8);
    buffer[2] = (uint8_t)(linecoding.bitrate >> 16);
    buffer[3] = (uint8_t)(linecoding.bitrate >> 24);
    buffer[4] = linecoding.format;
    buffer[5] = linecoding.paritytype;
    buffer[6] = linecoding.datatype;

    /* Add your code here */
    break;

  case CDC_SET_CONTROL_LINE_STATE:
    /* Add your code here */
    break;

  case CDC_SEND_BREAK:
     /* Add your code here */
    break;

  default:
    break;
  }

  return (0);
}

int8_t USB_Interface_Read(uint8_t* buffer, uint32_t* length)
{
  USB_OnReceivePacket(buffer, *length);

  return 0;
}

__attribute__((weak)) void USB_OnReceivePacket(uint8_t* buffer, uint32_t length)
{
  // No-op, weak symbol
}

// Initialize the USB device and API
void InitUSB(void)
{
  USBD_Init(&USBD_Device, &VCP_Desc, 0);
  USBD_RegisterClass(&USBD_Device, &USBD_CDC);
  USBD_CDC_RegisterInterface(&USBD_Device,
                             &USB_Interface);

  HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
  HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}

void USB_ReadyToReceive(void)
{
  USBD_CDC_ReceivePacket(&USBD_Device);
}

void USB_Write_Task(void* arg)
{
  Packet_t packet;
  uint8_t buffer[512];
  uint32_t offset, len;
  USBD_CDC_HandleTypeDef   *hcdc;

  UNUSED_ARG(arg);

  InitUSB();

  // Wait for other tasks to start up
  vTaskDelay(100);
  // Activate the USB device
  USBD_Start(&USBD_Device);
  // Wait for USB enumeration to complete
  vTaskDelay(500);

  // Wait until we have a device
  while ((hcdc = (USBD_CDC_HandleTypeDef*)USBD_Device.pClassData) == 0)
  {
    __DMB();
    vTaskDelay(100);
  }

  // Now begin the interface loop
  while (1)
  {
    while (xQueueReceive(usbWriteData.Tx, &packet, -1) != pdTRUE);

    // I should turn this into an RTOS event variable
    while (hcdc->TxState != 0)
      // Wait 1 ms for the transmitter to finish
      vTaskDelay(1);

    // Decompose the packet in USB transmission units
    offset = 0;
    while (offset < packet.Length)
    {
      // Calculate the next transmission unit size, =min(MTU, remaining data)
      len = egb_min(CDC_DATA_FS_MAX_PACKET_SIZE, packet.Length - offset);
      // Copy the data to the Tx buffer
      memcpy(buffer, &(packet.Data[offset]), len);
      offset += len;
      // Send via USB
      USBD_CDC_SetTxBuffer(&USBD_Device, buffer, len);
      USBD_CDC_TransmitPacket(&USBD_Device);

      // Wait for the transmission to complete
      while (hcdc->TxState != 0)
        vTaskDelay(1);
    }
    // Clean up the outbound packet
    packet_free(&packet);
  }
}
