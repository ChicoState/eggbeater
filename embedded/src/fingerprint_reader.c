#include "fingerprint_reader.h"

#include <gt511/gt511c1r.h>

#include <stm32f4xx_hal_rcc.h>
#include <stm32f4xx_hal_dma.h>
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_uart.h>

#include <stm32f429i_discovery.h>

#define FP_UART_RX_PIN  GPIO_PIN_2
#define FP_UART_RX_GPIO GPIOD

#define FP_UART_TX_PIN  GPIO_PIN_12
#define FP_UART_TX_GPIO GPIOC

#define __FP_UART_RX_GPIO_CLK_ENABLE() __GPIOD_CLK_ENABLE()
#define __FP_UART_TX_GPIO_CLK_ENABLE() __GPIOC_CLK_ENABLE()
#define __FP_UART_CLK_ENABLE()  __UART5_CLK_ENABLE()

#define FP_UART_GPIO    GPIOF
#define FP_UART_DEV     UART5
#define FP_UART_GPIO_AF GPIO_AF8_UART5

UART_HandleTypeDef uartEndpoint;
GT511C1R_Device_t gt511;

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

void Fingerprint_Task(void* arg)
{
  TickType_t lastWake = 0;
  uint32_t gt511Err = -1;
  uint32_t i = 0;
  uint32_t id = 0;
  (void)(arg);
  (void)(gt511Err);

  InitUART();

  BSP_LED_Off(LED3);

  vTaskDelay(50);

  GT511C1R_Init(&gt511, &uartEndpoint);

  BSP_LED_On(LED3);

  while (gt511Err != GT511C1R_ErrorCode_Success)
  {
    BSP_LED_On(LED3);
    gt511Err = GT511C1R_Open(&gt511);
    BSP_LED_Off(LED3);

    vTaskDelay(1000);
  }

  BSP_LED_Off(LED3);

  (void)(uartEndpoint.ErrorCode);

  GT511C1R_DeleteAllTemplates(&gt511);

  BSP_LED_On(LED3);

  id = fp_session_new();

  BSP_LED_Off(LED3);

  GT511C1R_DeleteOneTemplate(&gt511, id);

  lastWake = xTaskGetTickCount();

  while (1)
  {
    BSP_LED_Toggle(LED3);

    gt511Err = GT511C1R_LED(&gt511, i);

    i ^= 1;

    vTaskDelayUntil(&lastWake, 500);
  }
}

uint32_t fp_session_new()
{
  uint32_t id = 0;
  uint32_t gt511Ret = 0;
  // Find unused DB location

  for (; id < 20; id++)
  {
    gt511Ret = GT511C1R_IsIDInUse(&gt511, id);

    if (gt511Ret == 0)
    {
      break;
    }
  }

  if (id == 20)
  {
    // Database is full
    return -1;
  }

  // Send EnrollStart
  gt511Ret = GT511C1R_StartEnrollment(&gt511, id);

  if (gt511Ret != 0)
    return gt511Ret;

  // Turn on the scanner backlight
  gt511Ret = GT511C1R_LED(&gt511, 1);

  if (gt511Ret != 0)
    return gt511Ret;

  // Wait for finger down
  while ((gt511Ret = GT511C1R_IsFingerDown(&gt511)) != 1)
  {
    vTaskDelay(50);
  }

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
  while ((gt511Ret = GT511C1R_IsFingerDown(&gt511)) != 0)
  {
    vTaskDelay(50);
  }

  // Turn on the scanner backlight
  gt511Ret = GT511C1R_LED(&gt511, 1);

  if (gt511Ret != 0)
    return gt511Ret;

  // Wait for finger down
  while ((gt511Ret = GT511C1R_IsFingerDown(&gt511)) != 1)
  {
    vTaskDelay(50);
  }

  // Send CaptureFinger
  gt511Ret = GT511C1R_CaptureFingerprint(&gt511, 1);

  if (gt511Ret != 0)
    return gt511Ret;

  // Send Enroll2
  gt511Ret = GT511C1R_EnrollmentStep2(&gt511);

  if (gt511Ret != 0)
    return gt511Ret;

  // Wait for finger up
  while ((gt511Ret = GT511C1R_IsFingerDown(&gt511)) != 0)
  {
    vTaskDelay(50);
  }

  // Turn on the scanner backlight
  gt511Ret = GT511C1R_LED(&gt511, 1);

  if (gt511Ret != 0)
    return gt511Ret;

  // Wait for finger down
  while ((gt511Ret = GT511C1R_IsFingerDown(&gt511)) != 1)
  {
    vTaskDelay(50);
  }

  // Send CaptureFinger
  gt511Ret = GT511C1R_CaptureFingerprint(&gt511, 1);

  if (gt511Ret != 0)
    return gt511Ret;

  // Send Enroll3
  gt511Ret = GT511C1R_EnrollmentStep3(&gt511);

  if (gt511Ret != 0)
    return gt511Ret;

  return id;
}

