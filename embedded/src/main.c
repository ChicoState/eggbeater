/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f4xx.h"

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <usbd_core.h>
#include <usbd_cdc.h>
#include <usbd_desc.h>

#include "usb_interface.h"

//#include <stm32f4xx_hal_conf.h>

#include <stm32f429i_discovery.h>
#include <stm32f429i_discovery_lcd.h>

#ifndef UNUSED_ARG
  #define UNUSED_ARG(x) ( (void) ( x ) )
#endif // UNUSED_ARG

#define TASK_STACK_DEPTH  1024
#define USB_TASK_PRIO     2
#define ECHO_TASK_PRIO    3

#define LCD_FRAME_BUFFER_LAYER0 ( LCD_FRAME_BUFFER )
#define LCD_FRAME_BUFFER_LAYER1 ( LCD_FRAME_BUFFER + BUFFER_OFFSET )

USBD_HandleTypeDef USBD_Device;

void InitClocks(void);
void InitUSB(void);
void InitLCD(void);

// FreeRTOS task functions
void USB_Write_Task(void*);
void Echo_Task(void*);

void xPortSysTickHandler(void);

long OTG_ShouldYield = 0;

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

int main(void)
{
  HAL_Init();
  InitClocks();

  InitUSB();

  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  xTaskCreate(&USB_Write_Task,
              "USB Write Task",
              TASK_STACK_DEPTH,
              NULL,
              USB_TASK_PRIO,
              NULL);

  xTaskCreate(&Echo_Task,
              "Echo Task",
              TASK_STACK_DEPTH,
              NULL,
              ECHO_TASK_PRIO,
              NULL);

  usbWriteData.TransmitQueue  = xQueueCreate(16, sizeof(USB_Packet));
  usbWriteData.ReceiveQueue   = xQueueCreate(16, sizeof(USB_Packet));

  vTaskStartScheduler();

  while (1);
}

void InitClocks(void)
{
  RCC_ClkInitTypeDef clk;
  RCC_OscInitTypeDef osc;

  __PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  osc.HSEState = RCC_HSE_ON;
  osc.PLL.PLLState = RCC_PLL_ON;
  osc.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  osc.PLL.PLLM = 8;
  osc.PLL.PLLN = 336;
  osc.PLL.PLLP = RCC_PLLP_DIV2;
  osc.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&osc);

  clk.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

  clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clk.APB1CLKDivider = RCC_HCLK_DIV4;
  clk.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_5);
}

void InitUSB(void)
{
  USBD_Init(&USBD_Device, &VCP_Desc, 0);
  USBD_RegisterClass(&USBD_Device, &USBD_CDC);
  USBD_CDC_RegisterInterface(&USBD_Device,
                             &USB_Interface);

  HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}

void InitLCD()
{
  /* Initialize the LCD */
  BSP_LCD_Init();

  /* Layer2 Init */
  BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER_LAYER1);
  /* Set Foreground Layer */
  BSP_LCD_SelectLayer(1);
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetColorKeying(1, LCD_COLOR_WHITE);
  BSP_LCD_SetLayerVisible(1, DISABLE);

  /* Layer1 Init */
  BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER_LAYER0);

  /* Set Foreground Layer */
  BSP_LCD_SelectLayer(0);

  /* Enable The LCD */
  BSP_LCD_DisplayOn();

  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  BSP_LCD_SetFont(&Font16);

}

void USB_Write_Task(void* arg)
{
  USB_Packet packet;
  uint8_t buffer[512];
  USBD_CDC_HandleTypeDef   *hcdc;

  UNUSED_ARG(arg);

  BSP_LED_On(LED3);

  USBD_Start(&USBD_Device);

  InitLCD();
  /*
  BSP_LCD_Init();

  BSP_LCD_DisplayOff();
  BSP_LCD_DisplayOn();

  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  */
  BSP_LCD_ClearStringLine(0);
  BSP_LCD_DisplayStringAtLine(0, (uint8_t*)"USB Write task ready");

  hcdc = (USBD_CDC_HandleTypeDef*)USBD_Device.pClassData;

  while (1)
  {
    BSP_LCD_ClearStringLine(1);
    BSP_LCD_DisplayStringAtLine(1, (uint8_t*)"Waiting on packet to send");

    while (xQueueReceive(usbWriteData.TransmitQueue, &packet, -1) != pdTRUE);

    BSP_LCD_ClearStringLine(2);
    BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"Found packet, waiting to send");

    while (hcdc->TxState != 0);

    BSP_LCD_ClearStringLine(1);
    BSP_LCD_DisplayStringAtLine(1, (uint8_t*)"Found packet, ready to send");

    BSP_LED_Toggle(LED3);

    // Don't send long stuff yet
    memcpy(buffer, packet.Data, packet.Length);

    USBD_CDC_SetTxBuffer(&USBD_Device, buffer, packet.Length);
    USBD_CDC_TransmitPacket(&USBD_Device);

    BSP_LCD_ClearStringLine(1);
    BSP_LCD_DisplayStringAtLine(1, (uint8_t*)"Found packet, sent");

    free(packet.Data);
  }
}

void Echo_Task(void* arg)
{
  USB_Packet packet;
  UNUSED_ARG(arg);

  BSP_LED_On(LED4);

  while (1)
  {
    while (xQueueReceive(usbWriteData.ReceiveQueue, &packet, 5) != pdTRUE)
      USBD_CDC_ReceivePacket(&USBD_Device);

    BSP_LED_Toggle(LED4);

    while (xQueueSendToBack(usbWriteData.TransmitQueue, &packet, -1) != pdTRUE);
  }
}

void USB_OnReceivePacket(uint8_t* buffer, uint32_t length)
{
  USB_Packet packet;
  packet.Data   = malloc(length * sizeof(uint8_t));
  memcpy(packet.Data, buffer, length * sizeof(uint8_t));
  packet.Length = length;
  OTG_ShouldYield = 0;
  xQueueSendToBackFromISR(usbWriteData.ReceiveQueue, &packet, &OTG_ShouldYield);
}

void SysTick_Handler(void)
{
  HAL_IncTick();

  xPortSysTickHandler();
}

void _exit(int status)
{
    (void)(status);
    NVIC_SystemReset();

    while (1);
}
