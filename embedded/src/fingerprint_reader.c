#include "fingerprint_reader.h"

#include <gt511/gt511c1r.h>

#include <stm32f4xx_hal_rcc.h>
#include <stm32f4xx_hal_dma.h>
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_uart.h>

#include <stm32f429i_discovery.h>
#include <stm32f429i_discovery_lcd.h>

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

uint8_t* format_string(char* fmt, ...)
{
  static char buffer[65] = {0};

  va_list varArg;

  va_start(varArg, fmt);

  vsnprintf(buffer, 64, fmt, varArg);

  va_end(varArg);

  return (uint8_t*)buffer;
}

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

uint32_t fp_enroll_start(uint32_t id)
{
  uint32_t gt511Ret = 0;

  // Send EnrollStart
  gt511Ret = GT511C1R_StartEnrollment(&gt511, id);

  return gt511Ret;
}

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

uint32_t fp_get_rtc_time(void)
{
  RTC_TimeTypeDef sTime;
  uint32_t curTime;

  HAL_RTC_GetTime(&rtc, &sTime, FORMAT_BIN);

  curTime = (sTime.Hours * 3600) + (sTime.Minutes * 60) + (sTime.Seconds);

  return curTime;
}

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

void Fingerprint_Task(void* arg)
{
  TickType_t lastWake = 0;
  uint32_t gt511Err = -1;
  uint32_t i = 0;
  uint32_t id = 0;
  (void)(arg);
  (void)(gt511Err);

  InitLCD();
  InitUART();

  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

  BSP_LED_Off(LED3);

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

  BSP_LED_Off(LED3);

  BSP_LCD_ClearStringLine(1);
  BSP_LCD_DisplayStringAtLine(1, (uint8_t*)"GT511 Open - done");

  (void)(uartEndpoint.ErrorCode);

  GT511C1R_DeleteAllTemplates(&gt511);

  BSP_LED_On(LED3);

  BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"GT511 New FP");
  id = fp_session_new();

  BSP_LED_Off(LED3);

  BSP_LCD_ClearStringLine(2);
  BSP_LCD_ClearStringLine(3);
  BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"GT511 New FP - done");
  BSP_LCD_DisplayStringAtLine(3, format_string("id=%d", id));

  BSP_LCD_DisplayStringAtLine(4, (uint8_t*)"GT511 Verify");

  id = fp_session_open();

  BSP_LCD_ClearStringLine(4);
  BSP_LCD_ClearStringLine(5);
  BSP_LCD_DisplayStringAtLine(4, (uint8_t*)"GT511 Verify - done");

  if (id < 20)
  {
    BSP_LCD_DisplayStringAtLine(4, (uint8_t*)"GT511 Verify - done");
    BSP_LCD_DisplayStringAtLine(5, format_string("id=%d", id));
  }
  else
  {
    BSP_LCD_DisplayStringAtLine(4, (uint8_t*)"GT511 Verify - error");
    BSP_LCD_DisplayStringAtLine(5, format_string("code=%4x", id));
  }

  while (1)
    vTaskDelay(-1);

/*
  //GT511C1R_DeleteOneTemplate(&gt511, id);

  lastWake = xTaskGetTickCount();

  while (1)
  {
    BSP_LED_Toggle(LED3);

    gt511Err = GT511C1R_LED(&gt511, i);

    i ^= 1;

    vTaskDelayUntil(&lastWake, 500);
  } // */
}

uint32_t fp_session_new(void)
{
  uint32_t id = 0;
  uint32_t gt511Ret = 0;

  id = fp_find_unused_id();

  gt511Ret = fp_enroll_start(id);

  if (gt511Ret != 0)
  {
    asm("bkpt #0");
    return gt511Ret;
  }

  gt511Ret = fp_enroll_step_1();

  if (gt511Ret != 0)
  {
    asm("bkpt #0");
    return gt511Ret;
  }

  do
  {
    gt511Ret = fp_enroll_step_2();
  } while (gt511Ret == GT511C1R_ErrorCode_NoFinger);

  //gt511Ret = fp_enroll_step_2();

  if (gt511Ret != 0)
  {
    asm("bkpt #0");
    return gt511Ret;
  }

  do
  {
    gt511Ret = fp_enroll_step_3();
  } while (gt511Ret == GT511C1R_ErrorCode_NoFinger);

  //gt511Ret = fp_enroll_step_3();

  if (gt511Ret != 0)
  {
    asm("bkpt #0");
    return gt511Ret;
  }

  return id;
}

uint32_t fp_session_open(void)
{
  uint32_t gt511Ret = 0;
  uint32_t id = 0;

  // Turn on the scanner backlight
  gt511Ret = GT511C1R_LED(&gt511, 1);

  if (gt511Ret != 0)
  {
    asm("bkpt #0");
    return gt511Ret;
  }

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
    asm("bkpt #0");
    return gt511Ret;
  }

  // Send Identify
  gt511Ret = GT511C1R_VerifyAny(&gt511);

  id = gt511Ret;

  // Turn off the scanner backlight
  gt511Ret = GT511C1R_LED(&gt511, 0);

  if (gt511Ret != 0)
  {
    asm("bkpt #0");
    return gt511Ret;
  }

  // Wait for finger up
  //fp_wait_finger(0, 50);

  return id;
}

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

uint32_t fp_session_close(uint32_t id)
{
  if (id >= 20)
    return fp_error_id_invalid;

  Session_ActivityStamps[id] = 0;

  return 0;
}

uint32_t fp_generate_file_key(uint32_t id)
{
  uint32_t curTime = fp_get_rtc_time();
  uint32_t sessionTime = Session_ActivityStamps[id];

  if ((curTime - sessionTime) > SESSION_TIMEOUT)
  {
    fp_session_close(id);
    return fp_session_expired;
  }

  //! @todo Generate the file key
  /*
    file key = SHA2_256(finger print template)
  */

  return 1;
}
