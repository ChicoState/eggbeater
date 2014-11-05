#include <gt511/gt511c1r.h>

#include <stm32f4xx.h>
#include <stm32f4xx_hal_conf.h>
#include <stm32f4xx_hal_uart.h>

#include <stdlib.h>
#include <string.h>

#define UART_TIMEOUT (uint32_t)0x800

uint32_t GT511C1R_Transmit(GT511C1R_Device_t* device,
                           uint8_t* txBuffer,
                           uint32_t txSize)
{
  uint32_t ret = 0;
  uint16_t checksum = 0;
  int i;

  for (i = 0; i < txSize - 2; i++)
  {
    checksum = checksum + txBuffer[i];
  }

  *(uint16_t*)(&(txBuffer[txSize - 2])) = checksum;

  ret = HAL_UART_Transmit(device->UART_Endpoint,
                          txBuffer,
                          txSize,
                          -1);

  if (ret != HAL_OK)
  {
    return GT511C1R_ErrorCode_UARTError;
  }

  return GT511C1R_ErrorCode_Success;
}

uint32_t GT511C1R_Receive(GT511C1R_Device_t* device,
                           uint8_t* rxBuffer,
                           uint32_t rxSize)
{
  uint32_t ret = 0;

  ret = HAL_UART_Receive(device->UART_Endpoint,
                         rxBuffer,
                         rxSize,
                         -1);

  return ret;
}

void GT511C1R_Init(GT511C1R_Device_t* device, void* uart)
{
  if (device != NULL)
  {
    device->UART_Endpoint = uart;
    device->IsOpen = 0;

    device->Lock = xSemaphoreCreateMutex();
  }
}

void GT511C1R_DeInit(GT511C1R_Device_t* device)
{
  if (device != NULL)
  {
    device->UART_Endpoint = NULL;
    device->IsOpen = 0;

    vSemaphoreDelete(device->Lock);
    device->Lock = NULL;
  }
}

uint32_t GT511C1R_SendCommand(GT511C1R_Device_t* device,
                              GT511C1R_Command cmd,
                              uint32_t param,
                              uint8_t* rxBuffer,
                              uint32_t sizeOfDataPacket)
{
  GT511C1R_CommandPacket_t packet;
  HAL_StatusTypeDef ret;
  int i;

  packet.SOF1 = 0x55;
  packet.SOF2 = 0xAA;
  packet.DeviceID = 0x0001;
  packet.Parameter = param;
  packet.CommandCode = (uint16_t)cmd;
  packet.Checksum = 0;

  for (i = 0; i < sizeof(GT511C1R_CommandPacket_t) - 2; i++)
  {
    packet.Checksum = packet.Checksum + ((uint8_t*)(&packet))[i];
  }

  ret = HAL_UART_Transmit(device->UART_Endpoint,
                          (uint8_t*)&packet,
                          sizeof(GT511C1R_CommandPacket_t),
                          UART_TIMEOUT);

  if (ret != HAL_OK)
  {
    return GT511C1R_ErrorCode_UARTError;
  }

  ret = HAL_UART_Receive(device->UART_Endpoint,
                        rxBuffer,
                        sizeOfDataPacket,
                        UART_TIMEOUT);

  if (ret != HAL_OK)
    return GT511C1R_ErrorCode_UARTError;

  return GT511C1R_ErrorCode_Success;
}

uint32_t GT511C1R_Open(GT511C1R_Device_t* device)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_Open,
                             0,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  return ret;
}

uint32_t GT511C1R_Close(GT511C1R_Device_t* device)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_Close,
                             0,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
      return ackPacket.Parameter;

    xSemaphoreTake(device->Lock, portMAX_DELAY);
    device->IsOpen = 1;
    xSemaphoreGive(device->Lock);
  }

  return ret;
}

uint32_t GT511C1R_IsOpen(GT511C1R_Device_t* device)
{
  uint32_t isOpen = 0;

  if (xSemaphoreTake(device->Lock, 0))
  {
    isOpen = device->IsOpen;
    xSemaphoreGive(device->Lock);
  }
  else
  {
    isOpen = -1;
  }

  return isOpen;
}

uint32_t GT511C1R_LED(GT511C1R_Device_t* device, uint32_t state)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_LED,
                             state,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
      return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_ChangeUARTBaud(GT511C1R_Device_t* device, uint32_t newBaud)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_ChangeUARTBaud,
                             newBaud,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
      return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_GetEnrollCount(GT511C1R_Device_t* device)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_GetEnrollCount,
                             0,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_IsIDInUse(GT511C1R_Device_t* device, uint32_t id)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_CheckEnrolled,
                             id,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_Ack)
      return 1;
    else
      if (ackPacket.Parameter == GT511C1R_ErrorCode_IDNotUsed)
        return 0;
      else
        return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_StartEnrollment(GT511C1R_Device_t* device, uint32_t id)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_StartEnroll,
                             id,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
      return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_EnrollmentStep1(GT511C1R_Device_t* device)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_EnrollStep1,
                             0,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
      return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_EnrollmentStep2(GT511C1R_Device_t* device)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_EnrollStep2,
                             0,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
      return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_EnrollmentStep3(GT511C1R_Device_t* device)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_EnrollStep3,
                             0,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
      return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_IsFingerDown(GT511C1R_Device_t* device)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_HaveFinger,
                             0,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_Ack)
      return !ackPacket.Parameter;
    else
      return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_DeleteOneTemplate(GT511C1R_Device_t* device, uint32_t id)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_DeleteOne,
                             id,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
      return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_DeleteAllTemplates(GT511C1R_Device_t* device)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_DeleteAll,
                             0,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
      return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_VerifyOne(GT511C1R_Device_t* device, uint32_t id)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_VerifyOne,
                             id,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
      return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_VerifyAny(GT511C1R_Device_t* device)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_VerifyAny,
                             0,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
      return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_VerifyOneStoredTemplate(GT511C1R_Device_t* device, uint32_t id, GT511C1R_Template_t* t)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint8_t* templateBuffer = 0;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_VerifyOneTemplate,
                             id,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
    {
      xSemaphoreGive(device->Lock);
      return ackPacket.Parameter;
    }
  }

  templateBuffer = malloc(sizeof(GT511C1R_Template_t) + 6);

  memset(templateBuffer, 0, sizeof(GT511C1R_Template_t) + 6);

  templateBuffer[0] = 0x55;
  templateBuffer[1] = 0xAA;
  templateBuffer[3] = 0x01;

  memcpy(&(templateBuffer[4]), t->Data, 506);

  ret = GT511C1R_Transmit(device,
                          templateBuffer,
                          sizeof(GT511C1R_Template_t) + 6);

  if (ret != GT511C1R_ErrorCode_Success)
  {
    xSemaphoreGive(device->Lock);
    return ret;
  }

  ret = GT511C1R_Receive(device,
                         (uint8_t*)&ackPacket,
                         sizeof(ackPacket));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
      return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_VerifyAnyStoredTemplate(GT511C1R_Device_t* device, GT511C1R_Template_t* t)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint8_t* templateBuffer = 0;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_VerifyAnyTemplate,
                             0,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
    {
      xSemaphoreGive(device->Lock);
      return ackPacket.Parameter;
    }
  }

  templateBuffer = malloc(sizeof(GT511C1R_Template_t) + 6);

  memset(templateBuffer, 0, sizeof(GT511C1R_Template_t) + 6);

  templateBuffer[0] = 0x55;
  templateBuffer[1] = 0xAA;
  templateBuffer[3] = 0x01;

  memcpy(&(templateBuffer[4]), t->Data, 506);

  ret = GT511C1R_Transmit(device,
                          templateBuffer,
                          sizeof(GT511C1R_Template_t) + 6);


  free(templateBuffer);

  if (ret != GT511C1R_ErrorCode_Success)
  {
    xSemaphoreGive(device->Lock);
    return ret;
  }

  ret = GT511C1R_Receive(device,
                         (uint8_t*)&ackPacket,
                         sizeof(ackPacket));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
      return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_CaptureFingerprint(GT511C1R_Device_t* device, uint32_t quality)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_CaptureFinger,
                             quality,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
      return ackPacket.Parameter;
  }

  return ret;
}

uint32_t GT511C1R_GetCapturedFingerprintTemplate(GT511C1R_Device_t* device, GT511C1R_Template_t* t)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint8_t rxBuffer[sizeof(GT511C1R_Template_t) + 6] = {0};
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_MakeTemplateFromCapturedFinger,
                             0,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
    {
      xSemaphoreGive(device->Lock);
      return ackPacket.Parameter;
    }
  }

  ret = GT511C1R_Receive(device,
                         rxBuffer,
                         sizeof(GT511C1R_Template_t) + 6);

  xSemaphoreGive(device->Lock);

  memcpy(t->Data, &(rxBuffer[4]), sizeof(GT511C1R_Template_t));

  return ret;
}

uint32_t GT511C1R_GetStoredTemplate(GT511C1R_Device_t* device, uint32_t id, GT511C1R_Template_t* t)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint8_t rxBuffer[sizeof(GT511C1R_Template_t) + 6] = {0};
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_GetTemplate,
                             id,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
    {
      xSemaphoreGive(device->Lock);
      return ackPacket.Parameter;
    }
  }

  ret = GT511C1R_Receive(device,
                         rxBuffer,
                         sizeof(GT511C1R_Template_t) + 6);

  xSemaphoreGive(device->Lock);

  memcpy(t->Data, &(rxBuffer[4]), sizeof(GT511C1R_Template_t));

  return ret;
}

uint32_t GT511C1R_SetStoredTemplate(GT511C1R_Device_t* device, uint32_t id, GT511C1R_Template_t* t)
{
  GT511C1R_CommandPacket_t ackPacket;
  uint8_t txBuffer[sizeof(GT511C1R_Template_t) + 6] = {0};
  uint32_t ret = 0;

  if (device == NULL)
    return GT511C1R_ErrorCode_BadDevice;

  xSemaphoreTake(device->Lock, portMAX_DELAY);

  ret = GT511C1R_SendCommand(device,
                             GT511C1R_Command_SetTemplate,
                             0,
                             (uint8_t*)&ackPacket,
                             sizeof(GT511C1R_CommandPacket_t));

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
    {
      xSemaphoreGive(device->Lock);
      return ackPacket.Parameter;
    }
  }

  txBuffer[0] = 0x55;
  txBuffer[1] = 0xAA;
  txBuffer[3] = 0x01;

  memcpy(&(txBuffer[4]), t->Data, sizeof(GT511C1R_Template_t));

  ret = GT511C1R_Transmit(device,
                          txBuffer,
                          sizeof(GT511C1R_Template_t) + 6);

  if (ret != GT511C1R_ErrorCode_Success)
  {
    xSemaphoreGive(device->Lock);
    return ret;
  }

  ret = GT511C1R_Receive(device,
                         (uint8_t*)&ackPacket,
                         sizeof(ackPacket));

  xSemaphoreGive(device->Lock);

  if (ret == GT511C1R_ErrorCode_Success)
  {
    if (ackPacket.CommandCode == GT511C1R_Command_NotAck)
      return ackPacket.Parameter;
  }

  return ret;
}
