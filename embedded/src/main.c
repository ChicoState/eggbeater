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
#include <stm32f4xx.h>

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <usbd_core.h>
#include <usbd_cdc.h>
#include <usbd_desc.h>

#include "usb_interface.h"
#include "keypad.h"

//#include <stm32f4xx_hal_conf.h>

#include <stm32f429i_discovery.h>
#include <stm32f429i_discovery_lcd.h>
#include <stm32f429i_discovery_ts.h>

#ifndef UNUSED_ARG
  #define UNUSED_ARG(x) ( (void) ( x ) )
#endif // UNUSED_ARG

#define TASK_STACK_DEPTH  1024
#define USB_TASK_PRIO     4
#define ECHO_TASK_PRIO    3
#define KP_TASK_PRIO      2

#define LCD_FRAME_BUFFER_LAYER0 ( LCD_FRAME_BUFFER )
#define LCD_FRAME_BUFFER_LAYER1 ( LCD_FRAME_BUFFER + BUFFER_OFFSET )

// Because min is crazy
#define egb_min(a, b) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )

USBD_HandleTypeDef USBD_Device;

void InitClocks(void);
void InitUSB(void);
void InitLCD(void);

// FreeRTOS task functions
void USB_Write_Task(void*);
void Echo_Task(void*);
void Keypad_Task(void*);

void xPortSysTickHandler(void);

BaseType_t OTG_ShouldYield = 0;

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

  xTaskCreate(&Keypad_Task,
              "Echo Task",
              TASK_STACK_DEPTH,
              NULL,
              KP_TASK_PRIO,
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

  BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
}

void USB_Write_Task(void* arg)
{
  USB_Packet packet;
  uint8_t buffer[512];
  uint32_t offset, len;
  USBD_CDC_HandleTypeDef   *hcdc;

  UNUSED_ARG(arg);

  BSP_LED_On(LED3);

  USBD_Start(&USBD_Device);

  //InitLCD();

  //BSP_LCD_ClearStringLine(0);
  //BSP_LCD_DisplayStringAtLine(0, (uint8_t*)"USB Write task ready");

  //hcdc = (USBD_CDC_HandleTypeDef*)USBD_Device.pClassData;

  while ((hcdc = (USBD_CDC_HandleTypeDef*)USBD_Device.pClassData) == 0)
  {
    __DMB();
    vTaskDelay(100);
  }

  while (1)
  {
    while (xQueueReceive(usbWriteData.TransmitQueue, &packet, -1) != pdTRUE);

    while (hcdc->TxState != 0) // I should turn this into an RTOS event variable
      vTaskDelay(1); // Wait 1 ms for the transmitter to finish

    BSP_LED_Toggle(LED3);

    offset = 0;

    while (offset < packet.Length)
    {
      len = egb_min(CDC_DATA_FS_MAX_PACKET_SIZE, packet.Length - offset);
      memcpy(buffer, &(packet.Data[offset]), len);
      offset += len;

      USBD_CDC_SetTxBuffer(&USBD_Device, buffer, len);
      USBD_CDC_TransmitPacket(&USBD_Device);

      while (hcdc->TxState != 0)
        vTaskDelay(1);
    }

    //BSP_LCD_ClearStringLine(1);
    //BSP_LCD_DisplayStringAtLine(1, packet.Data);

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

void Keypad_Task(void* arg)
{
  TickType_t lastWake = 0;
  keypad_t kp;

  UNUSED_ARG(arg);

  InitLCD();

  keypad_init(&kp, &Font16, 20, 230);

  while (1)
  {
    //BSP_LED_Toggle(LED3);
    keypad_draw(&kp);
    keypad_checktouch(&kp);

    BSP_LCD_DisplayStringAtLine(3, (uint8_t*)kp.buffer);

    vTaskDelayUntil(&lastWake, 5);
  }
}

void USB_OnReceivePacket(uint8_t* buffer, uint32_t length)
{
  USB_Packet packet;

  packet.Data   = malloc((length + 1) * sizeof(uint8_t));
  memcpy(packet.Data, buffer, length * sizeof(uint8_t));

  packet.Data[length] = 0;
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