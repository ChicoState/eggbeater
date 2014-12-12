#include <stm32f4xx.h>

#include <stm32f429i_discovery.h>
#include <stm32f429i_discovery_lcd.h>
#include <stm32f429i_discovery_ts.h>

#include "common.h"

#include "usb_interface.h"
#include "Keypad.h"
#include "fingerprint_reader.h"
#include "Control.h"

#define TASK_STACK_DEPTH  512
#define FP_TASK_PRIO      5
#define USB_TASK_PRIO     4
#define CTRL_TASK_PRIO    3
#define KP_TASK_PRIO      2

void InitClocks(void);
void InitUSB(void);
void InitLCD(void);

// FreeRTOS task functions
void Control_Task(void*);

void xPortSysTickHandler(void);

extern BaseType_t OTG_ShouldYield;
/*
    Conditions:
      1
    Exit points:
      0
    M = 1 - 0 + 2 = 3
    Cyclomatic complexity
      3
  */
int main(void)
{
  HAL_Init();
  InitClocks();

  //*
  xTaskCreate(&USB_Write_Task,
              "USB Write Task",
              TASK_STACK_DEPTH,
              NULL,
              USB_TASK_PRIO,
              NULL);

  xTaskCreate(&Control_Task,
              "Control Task",
              TASK_STACK_DEPTH,
              NULL,
              CTRL_TASK_PRIO,
              NULL);
  // */

  /*
  xTaskCreate(&Keypad_Task,
              "Keypad Task",
              TASK_STACK_DEPTH,
              NULL,
              KP_TASK_PRIO,
              NULL);
  // */

  //*
  xTaskCreate(&Fingerprint_Task,
              "FP Task",
              TASK_STACK_DEPTH,
              NULL,
              FP_TASK_PRIO,
              NULL);
  // */

  usbWriteData.Tx  = xQueueCreate(16, sizeof(Packet_t));
  usbWriteData.Rx   = xQueueCreate(16, sizeof(Packet_t));

  vTaskStartScheduler();

  while (1);
}
/*
    Conditions:
      0
    Exit points:
      0
    M = 0 - 0 + 2 = 2
    Cyclomatic complexity
      2
  */
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
/*
    Conditions:
      0
    Exit points:
      0
    M = 0 - 0 + 2 = 2
    Cyclomatic complexity
      2
  */
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
/*
    Conditions:
      7
    Exit points:
      0
    M = 7 - 0 + 2 = 9
    Cyclomatic complexity
      9
  */
void Control_Task(void* arg)
{
  Packet_t packet, resp;
  UNUSED_ARG(arg);

  control_init(&ctrlState);

  while (1)
  {
    while (xQueueReceive(usbWriteData.Rx, &packet, 50) != pdTRUE)
      USB_ReadyToReceive();

    //! @todo Reconstruct fractured packets, as needed

    if (control_should_handle_packet(&ctrlState, &packet) == CtrlError_Success)
    {
      if (control_handle_packet(&ctrlState, &packet) == CtrlError_Success)
      {
        // Check if a response packet is expected
        if (control_is_packet_ready(&ctrlState, &packet) != CtrlError_NotReady)
        {
          while (control_is_packet_ready(&ctrlState, &packet) != CtrlError_Yes)
          {
            vTaskDelay(5);
          }

          control_get_resp_packet(&ctrlState, &resp);

          while (xQueueSendToBack(usbWriteData.Tx, &resp, -1) != pdTRUE);
        }
      }
    }

    packet_free(&packet);
  }
}
/*
    Conditions:
      0
    Exit points:
      0
    M = 0 - 0 + 2 = 2
    Cyclomatic complexity
      2
  */
void USB_OnReceivePacket(uint8_t* buffer, uint32_t length)
{
  Packet_t packet;

  packet.Data   = malloc((length + 1) * sizeof(uint8_t));
  memcpy(packet.Data, buffer, length * sizeof(uint8_t));

  packet.Data[length] = 0;
  packet.Length = length;

  OTG_ShouldYield = 0;

  xQueueSendToBackFromISR(usbWriteData.Rx, &packet, &OTG_ShouldYield);
}
/*
    Conditions:
      0
    Exit points:
      0
    M = 0 - 0 + 2 = 2
    Cyclomatic complexity
      2
  */
void SysTick_Handler(void)
{
  HAL_IncTick();

  xPortSysTickHandler();
}
/*
    Conditions:
      1
    Exit points:
      0
    M = 1 - 0 + 2 = 3
    Cyclomatic complexity
      3
  */
void _exit(int status)
{
  UNUSED_ARG(status);
  NVIC_SystemReset();

  while (1);
}
