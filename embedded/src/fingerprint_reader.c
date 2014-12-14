#include "fingerprint_reader.h"
#include "Control.h"
#include "GUI_Button.h"

#include "polarssl/sha256.h"

#include <gt511/gt511c1r.h>

#include <stm32f4xx_hal_rcc.h>
#include <stm32f4xx_hal_dma.h>
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_uart.h>

#include <stm32f429i_discovery.h>
#include <stm32f429i_discovery_lcd.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define FP_UART_RX_PIN  GPIO_PIN_2
#define FP_UART_RX_GPIO GPIOD

#define FP_UART_TX_PIN  GPIO_PIN_12
#define FP_UART_TX_GPIO GPIOC

#define __FP_UART_RX_GPIO_CLK_ENABLE()  __GPIOD_CLK_ENABLE()
#define __FP_UART_TX_GPIO_CLK_ENABLE()  __GPIOC_CLK_ENABLE()
#define __FP_UART_CLK_ENABLE()          __UART5_CLK_ENABLE()

#define FP_UART_GPIO    GPIOF
#define FP_UART_DEV     UART5
#define FP_UART_GPIO_AF GPIO_AF8_UART5

#define SESSION_TIMEOUT ( 10 * 60 )

RTC_HandleTypeDef   rtc;
UART_HandleTypeDef  uartEndpoint;
GT511C1R_Device_t   gt511;

uint32_t Session_ActivityStamps[20] = {0};

uint32_t fp_session_start(void);
void fp_test(void);
/*
    Conditions:
      0
    Exit points:
      1
    M = 0 - 1 + 2 = 1
    Cyclomatic complexity
      1
  */
uint8_t* format_string(char* fmt, ...)
{
  static char buffer[65] = {0};

  va_list varArg;

  va_start(varArg, fmt);

  vsnprintf(buffer, 64, fmt, varArg);

  va_end(varArg);

  return (uint8_t*)buffer;
}
/*
    Conditions:
      2
    Exit points:
      2
    M = 2 - 2 + 2 = 2
    Cyclomatic complexity
      2
  */
uint32_t fp_find_unused_id(void)
{
  uint32_t id = 0;
  uint32_t gt511Ret = 0;

  // For each valid ID
  for (; id < 20; id++)
  {
    // Check if in use
    gt511Ret = GT511C1R_IsIDInUse(&gt511, id);

    // If not
    if (gt511Ret == 0)
    {
      // Use it
      return id;
    }
  }

  return -1;
}
/*
    Conditions:
      1
    Exit points:
      1
    M = 1 - 1 + 2 = 2
    Cyclomatic complexity
      2
  */
uint32_t fp_wait_finger(uint32_t state, uint32_t delay)
{
  uint32_t gt511Ret = 0;

  // Sanitize to exactly 1 or 0
  state = (state) ? (1) : (0);

  // Wait for finger down
  while ((gt511Ret = GT511C1R_IsFingerDown(&gt511)) != state)
  {
    vTaskDelay(delay);
  }

  return gt511Ret;
}
/*
    Conditions:
      0
    Exit points:
      1
    M = 0 - 1 + 2 = 1
    Cyclomatic complexity
      1
  */
uint32_t fp_enroll_start(uint32_t id)
{
  uint32_t gt511Ret = 0;

  // Send EnrollStart
  gt511Ret = GT511C1R_StartEnrollment(&gt511, id);

  return gt511Ret;
}
/*
    Conditions:
      4
    Exit points:
      5
    M = 4 - 5 + 2 = 1
    Cyclomatic complexity
      1
  */
uint32_t fp_enroll_step_1(void)
{
  uint32_t gt511Ret;

  // Turn on the scanner backlight
  gt511Ret = GT511C1R_LED(&gt511, 1);

  if (gt511Ret != 0)
    return gt511Ret;

  // Wait for finger down
  fp_wait_finger(1, 50);

  // Send CaptureFinger
  gt511Ret = GT511C1R_CaptureFingerprint(&gt511, 1);

  if (gt511Ret != 0)
    return gt511Ret;

  // Send Enroll1
  gt511Ret = GT511C1R_EnrollmentStep1(&gt511);

  if (gt511Ret != 0)
    return gt511Ret;

  // Turn off the scanner backlight
  gt511Ret = GT511C1R_LED(&gt511, 0);

  if (gt511Ret != 0)
    return gt511Ret;

  // Wait for finger up
  fp_wait_finger(0, 50);

  return gt511Ret;
}
/*
    Conditions:
      4
    Exit points:
      5
    M = 4 - 5 + 2 = 1
    Cyclomatic complexity
      1
  */
uint32_t fp_enroll_step_2(void)
{
  uint32_t gt511Ret;

  // Turn on the scanner backlight
  gt511Ret = GT511C1R_LED(&gt511, 1);

  if (gt511Ret != 0)
    return gt511Ret;

  // Wait for finger down
  fp_wait_finger(1, 50);

  // Send CaptureFinger
  gt511Ret = GT511C1R_CaptureFingerprint(&gt511, 1);

  if (gt511Ret != 0)
    return gt511Ret;

  // Send Enroll1
  gt511Ret = GT511C1R_EnrollmentStep2(&gt511);

  if (gt511Ret != 0)
    return gt511Ret;

  // Turn off the scanner backlight
  gt511Ret = GT511C1R_LED(&gt511, 0);

  if (gt511Ret != 0)
    return gt511Ret;

  // Wait for finger up
  fp_wait_finger(0, 50);

  return gt511Ret;
}
/*
    Conditions:
      4
    Exit points:
      5
    M = 4 - 5 + 2 = 1
    Cyclomatic complexity
      1
  */
uint32_t fp_enroll_step_3(void)
{
  uint32_t gt511Ret;

  // Turn on the scanner backlight
  gt511Ret = GT511C1R_LED(&gt511, 1);

  if (gt511Ret != 0)
    return gt511Ret;

  // Wait for finger down
  fp_wait_finger(1, 50);

  // Send CaptureFinger
  gt511Ret = GT511C1R_CaptureFingerprint(&gt511, 1);

  if (gt511Ret != 0)
    return gt511Ret;

  // Send Enroll1
  gt511Ret = GT511C1R_EnrollmentStep3(&gt511);

  if (gt511Ret != 0)
    return gt511Ret;

  // Turn off the scanner backlight
  gt511Ret = GT511C1R_LED(&gt511, 0);

  if (gt511Ret != 0)
    return gt511Ret;

  // Wait for finger up
  fp_wait_finger(0, 50);

  return gt511Ret;
}
/*
    Conditions:
      0
    Exit points:
      1
    M = 0 - 1 + 2 = 3
    Cyclomatic complexity
      1
  */
uint32_t fp_get_rtc_time(void)
{
  RTC_TimeTypeDef sTime;
  uint32_t curTime;

  HAL_RTC_GetTime(&rtc, &sTime, FORMAT_BIN);

  curTime = (sTime.Hours * 3600) + (sTime.Minutes * 60) + (sTime.Seconds);

  return curTime;
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
void InitUART(void)
{
  GPIO_InitTypeDef gpio;

  __FP_UART_RX_GPIO_CLK_ENABLE();
  __FP_UART_TX_GPIO_CLK_ENABLE();
  __FP_UART_CLK_ENABLE();

  gpio.Mode = GPIO_MODE_AF_PP;
  gpio.Alternate = FP_UART_GPIO_AF;
  gpio.Speed = GPIO_SPEED_HIGH;
  gpio.Pull = GPIO_NOPULL;
  gpio.Pin = FP_UART_RX_PIN;

  HAL_GPIO_Init(FP_UART_RX_GPIO, &gpio);

  gpio.Mode = GPIO_MODE_AF_PP;
  gpio.Alternate = FP_UART_GPIO_AF;
  gpio.Speed = GPIO_SPEED_HIGH;
  gpio.Pull = GPIO_NOPULL;
  gpio.Pin = FP_UART_TX_PIN;

  HAL_GPIO_Init(FP_UART_TX_GPIO, &gpio);

  uartEndpoint.hdmarx = NULL;
  uartEndpoint.hdmatx = NULL;
  uartEndpoint.Instance = FP_UART_DEV;
  uartEndpoint.Init.BaudRate = 9600;
  uartEndpoint.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uartEndpoint.Init.Mode = UART_MODE_TX_RX;
  uartEndpoint.Init.OverSampling = UART_OVERSAMPLING_16;
  uartEndpoint.Init.Parity = UART_PARITY_NONE;
  uartEndpoint.Init.StopBits = UART_STOPBITS_1;
  uartEndpoint.Init.WordLength = UART_WORDLENGTH_8B;

  HAL_UART_Init(&uartEndpoint);

  BSP_LED_Init(LED3);
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
void InitRTC(void)
{
  __PWR_CLK_ENABLE();

  HAL_PWR_EnableBkUpAccess();

  __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_HSE_DIV16);

  __HAL_RCC_RTC_ENABLE();

  rtc.Instance = RTC;
  rtc.Init.AsynchPrediv = 0x63; // Step down the clock by a factor of 100
  rtc.Init.SynchPrediv = 0x1387; // Step down the clock by a factor of 5000
  rtc.Init.HourFormat = RTC_HOURFORMAT_24;

  rtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  rtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW;
  rtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
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
void gt511_startup(void)
{
  uint32_t gt511Err = -1;
  // GT511 startup sequence
  vTaskDelay(50);

  BSP_LCD_DisplayStringAtLine(0, (uint8_t*)"GT511 Init");

  GT511C1R_Init(&gt511, &uartEndpoint);

  BSP_LCD_ClearStringLine(0);
  BSP_LCD_DisplayStringAtLine(0, (uint8_t*)"GT511 Init - done");

  BSP_LCD_DisplayStringAtLine(1, (uint8_t*)"GT511 Open");

  BSP_LED_On(LED3);

  while (gt511Err != GT511C1R_ErrorCode_Success)
  {
    BSP_LED_On(LED3);
    gt511Err = GT511C1R_Open(&gt511);
    BSP_LED_Off(LED3);

    vTaskDelay(1000);
  }

  BSP_LCD_ClearStringLine(1);
  BSP_LCD_DisplayStringAtLine(1, (uint8_t*)"GT511 Open - done");
}
/*
    Conditions:
      18
    Exit points:
      1
    M = 18 - 1 + 2 = 19
    Cyclomatic complexity
      19
  */
uint32_t fp_handle_packet(Packet_t* packet, Packet_t* response)
{
  uint32_t id;
  uint32_t retVal = 0;
  PacketHeader_t* head = NULL;
  uint16_t len = 0;
  uint8_t* data = NULL;
  uint8_t digest[32] = {0};

  if (packet_get_header(packet, &head))
  {
    // Error
  }

  if (packet_get_data_length(packet, &len))
  {
    // Error
  }

  if (packet_get_data(packet, &data))
  {
    // Error
  }

  switch (head->Command)
  {
    default:
      // Error
      break;

    case CtrlCommand_OpenSession:
    case CtrlCommand_NewSession:
    {
      id = fp_session_start();

      if (packet_create(response, head->Command, (uint8_t*)&id, sizeof(id)))
      {
        // Error
      }
      break;
    }

    case CtrlCommand_RefreshSession:
    {
      if (len >= 4)
      {
        id = *(uint32_t*)data;
        fp_session_refresh(id);

        id = 0x01;
      }
      else
        id = fp_error_id_invalid;

      if (packet_create(response, head->Command, (uint8_t*)&id, sizeof(id)))
      {
        // Error
      }
      break;
    }

    case CtrlCommand_CloseSession:
    {
      if (len >= 4)
      {
        id = *(uint32_t*)data;
        fp_session_close(id);
      }
      break;
    }

    case CtrlCommand_GetFileKey:
    {
      if (len >= 4)
      {
        id = *(uint32_t*)data;
        if ((retVal = fp_generate_file_key(id, digest)) < 20)
        {
          if (packet_create(response, head->Command, (uint8_t*)&digest, sizeof(digest)))
          {
            // Error
          }
        }
        else
        {
          // Error message
          if (packet_create(response, head->Command, (uint8_t*)&retVal, sizeof(retVal)))
          {
            // Error
          }
        }
      }
      else
      {
        id = fp_error_id_invalid;

        if (packet_create(response, head->Command, (uint8_t*)&id, sizeof(id)))
        {
          // Error
        }
      }
      // Session ID is in data section
      break;
    }
  }

  return 0;
}
/*
    Conditions:
      9
    Exit points:
      0
    M = 9 - 0 + 2 = 11
    Cyclomatic complexity
      11
  */
void Fingerprint_Task(void* arg)
{
  //TickType_t lastWake = 0;
  Packet_t packet, response;
  uint32_t i = 0;

  UNUSED_ARG(arg);

  InitLCD();
  InitUART();

  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

  gt511_startup();

  response.Data = NULL;
  response.Length = 0;
  packet.Data = NULL;
  packet.Length = 0;

  //*
  //fp_test();
  // */

  while (1)
  {
    BSP_LCD_ClearStringLine(2);
    BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"Waiting on PC");

    i = 0;

    while (xQueueReceive(ctrlDataQueue.Rx, &packet, 500) != pdTRUE)
    {
      i += 1;

      BSP_LCD_ClearStringLine(2);

      switch (i & 3)
      {
        default:
        case 0:
          BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"Waiting on PC");
          break;
        case 1:
          BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"Waiting on PC.");
          break;
        case 2:
          BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"Waiting on PC..");
          break;
        case 3:
          BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"Waiting on PC...");
          break;
      }
    }

    fp_handle_packet(&packet, &response);

    if (response.Data != NULL)
      while (xQueueSendToBack(ctrlDataQueue.Tx, &response, -1) != pdTRUE);

    packet_free(&packet);

    response.Data = NULL;
    response.Length = 0;
    packet.Data = NULL;
    packet.Length = 0;

    BSP_LCD_Clear(LCD_COLOR_WHITE);
  }
}
/*
    Conditions:
      7
    Exit points:
      1
    M = 7 - 1 + 2 = 8
    Cyclomatic complexity
      8
  */
uint32_t fp_session_start(void)
{
  uint32_t id = 0;
  TickType_t lastWake = 0;
  uint32_t action = 0;
  TS_StateTypeDef ts;

  GUI_Button_t newID, existing;

  gui_button_init(&newID);
  newID.XPosition   = 20;
  newID.YPosition   = 100;
  newID.Height      = 75;
  newID.Width       = 200;
  newID.BorderWidth = 2;
  newID.BorderColor = LCD_COLOR_BLACK;
  newID.FillColor   = LCD_COLOR_WHITE;
  newID.TextColor   = LCD_COLOR_BLACK;
  newID.Text = (uint8_t*)"New user";

  gui_button_init(&existing);
  existing.XPosition   = 20;
  existing.YPosition   = 200;
  existing.Height      = 75;
  existing.Width       = 200;
  existing.BorderWidth = 2;
  existing.BorderColor = LCD_COLOR_BLACK;
  existing.FillColor   = LCD_COLOR_WHITE;
  existing.TextColor   = LCD_COLOR_BLACK;
  existing.Text = (uint8_t*)"Existing user";

  lastWake = xTaskGetTickCount();

  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /*
    Display buttons to user
      1. New ID
      2. Existing
  */

  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 3 / 2, (uint8_t*)"Authenticate", CENTER_MODE);

  gui_button_draw(&newID);
  gui_button_draw(&existing);

  // Wait for user selection
  while (1)
  {
    // If canceled,
    // Abort

    BSP_TS_GetState(&ts);

    if (gui_button_check_touch(&newID, &ts) == GUI_Button_Error_Yes)
    {
      action = 1;
      break;
    }
    else if (gui_button_check_touch(&existing, &ts) == GUI_Button_Error_Yes)
    {
      action = 2;
      break;
    }

    vTaskDelayUntil(&lastWake, 100);
  }

  //__asm__("bkpt #1");

  switch (action)
  {
    // On New ID,
    case 1:
      // Call fp_session_new
      id = fp_session_new();
      break;

    // On Existing,
    case 2:
      // Call fp_session_open
      id = fp_session_open();
      break;

    default:
      id = fp_error_session_not_started;
      break;
  }

  if (id < 20)
  {
    Session_ActivityStamps[id] = fp_get_rtc_time();
  }

  return id;
}
/*
    Conditions:
      6
    Exit points:
      5
    M = 6 - 5 + 2 = 3
    Cyclomatic complexity
      3
  */
uint32_t fp_session_new(void)
{
  uint32_t id = 0;
  uint32_t gt511Ret = 0;

  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 3 / 2, (uint8_t*)"Authenticate", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 5 / 2, (uint8_t*)"New user", CENTER_MODE);

  id = fp_find_unused_id();

  gt511Ret = fp_enroll_start(id);

  if (gt511Ret != 0)
  {
    __asm__("bkpt #0");
    return gt511Ret;
  }

  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 7 / 2, (uint8_t*)"Enrollment step 1", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 9 / 2, (uint8_t*)"Please place finger", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 11 / 2, (uint8_t*)"on reader", CENTER_MODE);

  gt511Ret = fp_enroll_step_1();

  if (gt511Ret != 0)
  {
    __asm__("bkpt #0");
    return gt511Ret;
  }

  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 3 / 2, (uint8_t*)"Authenticate", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 5 / 2, (uint8_t*)"New user", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 7 / 2, (uint8_t*)"Enrollment step 2", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 9 / 2, (uint8_t*)"Please place finger", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 11 / 2, (uint8_t*)"on reader", CENTER_MODE);

  do
  {
    gt511Ret = fp_enroll_step_2();
  } while (gt511Ret == GT511C1R_ErrorCode_NoFinger);

  //gt511Ret = fp_enroll_step_2();

  if (gt511Ret != 0)
  {
    __asm__("bkpt #0");
    return gt511Ret;
  }

  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 3 / 2, (uint8_t*)"Authenticate", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 5 / 2, (uint8_t*)"New user", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 7 / 2, (uint8_t*)"Enrollment step 3", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 9 / 2, (uint8_t*)"Please place finger", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 11 / 2, (uint8_t*)"on reader", CENTER_MODE);

  do
  {
    gt511Ret = fp_enroll_step_3();
  } while (gt511Ret == GT511C1R_ErrorCode_NoFinger);

  //gt511Ret = fp_enroll_step_3();

  if (gt511Ret != 0)
  {
    __asm__("bkpt #0");
    return gt511Ret;
  }

  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 3 / 2, (uint8_t*)"Authenticate", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 5 / 2, (uint8_t*)"New user", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 7 / 2, (uint8_t*)"Enrollment done", CENTER_MODE);

  return id;
}
/*
    Conditions:
      4
    Exit points:
      4
    M = 4 - 4 + 2 = 2
    Cyclomatic complexity
      2
  */
uint32_t fp_session_open(void)
{
  uint32_t gt511Ret = 0;
  uint32_t id = 0;

  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 3 / 2, (uint8_t*)"Authenticate", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 5 / 2, (uint8_t*)"Existing user", CENTER_MODE);

  // Turn on the scanner backlight
  gt511Ret = GT511C1R_LED(&gt511, 1);

  if (gt511Ret != 0)
  {
    __asm__("bkpt #0");
    return gt511Ret;
  }


  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 7 / 2, (uint8_t*)"Identify", CENTER_MODE);


  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 9 / 2, (uint8_t*)"Please place finger", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetFont()->Height * 11 / 2, (uint8_t*)"on reader", CENTER_MODE);

  // Wait for finger down
  fp_wait_finger(1, 50);

  do
  {
    // Send CaptureFinger
    gt511Ret = GT511C1R_CaptureFingerprint(&gt511, 1);
  } while (gt511Ret == GT511C1R_ErrorCode_NoFinger);
  // Send CaptureFinger
  //gt511Ret = GT511C1R_CaptureFingerprint(&gt511, 1);

  if (gt511Ret != 0)
  {
    __asm__("bkpt #0");
    return gt511Ret;
  }

  // Send Identify
  gt511Ret = GT511C1R_VerifyAny(&gt511);

  id = gt511Ret;

  // Turn off the scanner backlight
  gt511Ret = GT511C1R_LED(&gt511, 0);

  if (gt511Ret != 0)
  {
    __asm__("bkpt #0");
    return gt511Ret;
  }

  // id is error, display to user

  // Wait for finger up
  fp_wait_finger(0, 50);

  return id;
}
/*
    Conditions:
      4
    Exit points:
      5
    M = 4 - 5 + 2 = 1
    Cyclomatic complexity
      1
  */
uint32_t fp_session_refresh(uint32_t id)
{
  uint32_t curTime = fp_get_rtc_time();
  uint32_t sessionTime = Session_ActivityStamps[id];

  if (id >= 20)
    return fp_error_id_invalid;

  if (sessionTime == 0)
    return fp_error_session_not_started;

  if (sessionTime < curTime) // Somebody is fucking with me
  {
    fp_session_close(id);
    return fp_error_session_not_started;
  }

  if ((curTime - sessionTime) > SESSION_TIMEOUT)
  {
    fp_session_close(id);
    return fp_session_expired;
  }

  Session_ActivityStamps[id] = curTime;

  return 0;
}
/*
    Conditions:
      1
    Exit points:
      2
    M = 1 - 2 + 2 = 1
    Cyclomatic complexity
      1
  */
uint32_t fp_session_close(uint32_t id)
{
  if (id >= 20)
    return fp_error_id_invalid;

  Session_ActivityStamps[id] = 0;

  return 0;
}
/*
    Conditions:
      2
    Exit points:
      2
    M = 2 - 2 + 2 = 2
    Cyclomatic complexity
      2
  */
uint32_t fp_generate_file_key(uint32_t id, uint8_t* digest)
{
  uint32_t errCode = 0;
  uint32_t curTime = fp_get_rtc_time();
  uint32_t sessionTime; // = Session_ActivityStamps[id];
  GT511C1R_Template_t* t = NULL;

  if (id >= 20)
  {
    return fp_error_id_invalid;
  }
  sessionTime = Session_ActivityStamps[id];

  if ((curTime - sessionTime) > SESSION_TIMEOUT)
  {
    fp_session_close(id);
    return fp_session_expired;
  }

  t = malloc(sizeof(GT511C1R_Template_t));

  // Read fingerprint template from GT511
  if ((errCode = GT511C1R_GetStoredTemplate(&gt511, id, t)))
  {
    // Error
  }

  sha256(t->Data, sizeof(GT511C1R_Template_t), digest, 0);

  free(t);

  return 0;
}
/*
    Conditions:
      22
    Exit points:
      0
    M = 22 - 0 + 2 = 25
    Cyclomatic complexity
      25
  */
void fp_test(void)
{
  uint32_t retCode = 0;
  GUI_Button_t  fp_start,
                fp_refresh,
                fp_close,
                fp_continue;
  TS_StateTypeDef ts;
  uint32_t action = 0, id = 0;

  gui_button_init(&fp_start);
  fp_start.XPosition   = 20;
  fp_start.YPosition   = 50;
  fp_start.Height      = 50;
  fp_start.Width       = 200;
  fp_start.BorderWidth = 2;
  fp_start.BorderColor = LCD_COLOR_BLACK;
  fp_start.FillColor   = LCD_COLOR_WHITE;
  fp_start.TextColor   = LCD_COLOR_BLACK;
  fp_start.Text = (uint8_t*)"Start";

  gui_button_init(&fp_refresh);
  fp_refresh.XPosition   = 20;
  fp_refresh.YPosition   = 125;
  fp_refresh.Height      = 50;
  fp_refresh.Width       = 200;
  fp_refresh.BorderWidth = 2;
  fp_refresh.BorderColor = LCD_COLOR_BLACK;
  fp_refresh.FillColor   = LCD_COLOR_WHITE;
  fp_refresh.TextColor   = LCD_COLOR_BLACK;
  fp_refresh.Text = (uint8_t*)"Refresh";

  gui_button_init(&fp_close);
  fp_close.XPosition   = 20;
  fp_close.YPosition   = 200;
  fp_close.Height      = 50;
  fp_close.Width       = 200;
  fp_close.BorderWidth = 2;
  fp_close.BorderColor = LCD_COLOR_BLACK;
  fp_close.FillColor   = LCD_COLOR_WHITE;
  fp_close.TextColor   = LCD_COLOR_BLACK;
  fp_close.Text = (uint8_t*)"Close";

  gui_button_init(&fp_continue);
  fp_continue.XPosition   = 20;
  fp_continue.YPosition   = 250;
  fp_continue.Height      = 50;
  fp_continue.Width       = 200;
  fp_continue.BorderWidth = 2;
  fp_continue.BorderColor = LCD_COLOR_BLACK;
  fp_continue.FillColor   = LCD_COLOR_WHITE;
  fp_continue.TextColor   = LCD_COLOR_BLACK;
  fp_continue.Text = (uint8_t*)"Continue";

  while (1)
  {
    BSP_LCD_Clear(LCD_COLOR_WHITE);

    gui_button_draw(&fp_start);
    gui_button_draw(&fp_refresh);
    gui_button_draw(&fp_close);

    action = 0;

    while (action == 0)
    {
      BSP_TS_GetState(&ts);

      if (gui_button_check_touch(&fp_start, &ts) == GUI_Button_Error_Yes)
      {
        action = 1;
      }
      else if (gui_button_check_touch(&fp_refresh, &ts) == GUI_Button_Error_Yes)
      {
        action = 2;
      }
      else if (gui_button_check_touch(&fp_close, &ts) == GUI_Button_Error_Yes)
      {
        action = 3;
      }
      else
      {
        vTaskDelay(5);
      }
    }

    switch (action)
    {
      case 1:
        id = fp_session_start();
        BSP_LCD_Clear(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAtLine(1, (uint8_t*)"Session start");
        if (id < 20)
          BSP_LCD_DisplayStringAtLine(2, format_string("ID: %d", id));
        else
          BSP_LCD_DisplayStringAtLine(2, format_string("Error code: %x", id));

        gui_button_draw(&fp_continue);

        while (1)
        {
          BSP_TS_GetState(&ts);

          if (gui_button_check_touch(&fp_continue, &ts) == GUI_Button_Error_Yes)
            break;

          vTaskDelay(5);
        }
        break;

      case 2:
        retCode = fp_session_refresh(id);
        BSP_LCD_Clear(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAtLine(1, (uint8_t*)"Session refresh");
        if (retCode < 20)
          BSP_LCD_DisplayStringAtLine(2, format_string("ID: %d", id));
        else
          BSP_LCD_DisplayStringAtLine(2, format_string("Error code: %x", retCode));

        gui_button_draw(&fp_continue);

        while (1)
        {
          BSP_TS_GetState(&ts);

          if (gui_button_check_touch(&fp_continue, &ts) == GUI_Button_Error_Yes)
            break;

          vTaskDelay(5);
        }
        break;

      case 3:
        retCode = fp_session_close(id);
        BSP_LCD_Clear(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAtLine(1, (uint8_t*)"Session close");
        if (retCode < 20)
          BSP_LCD_DisplayStringAtLine(2, format_string("ID: %d", id));
        else
          BSP_LCD_DisplayStringAtLine(2, format_string("Error code: %x", retCode));

        gui_button_draw(&fp_continue);

        while (1)
        {
          BSP_TS_GetState(&ts);

          if (gui_button_check_touch(&fp_continue, &ts) == GUI_Button_Error_Yes)
            break;

          vTaskDelay(5);
        }
        break;

      default:
        id = fp_error_session_not_started;
        break;
    }
  }
}
