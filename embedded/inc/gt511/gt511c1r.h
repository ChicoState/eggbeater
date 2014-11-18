#ifndef _GT511_GT511C1R_H_
#define _GT511_GT511C1R_H_

#include <stdint.h>
#include <FreeRTOS.h>
#include <semphr.h>

typedef enum GT511C1R_Command
{
  GT511C1R_Command_Open                           = 0x01,
  GT511C1R_Command_Close                          = 0x02,
  GT511C1R_Command_ChangeUARTBaud                 = 0x04,

  GT511C1R_Command_LED                            = 0x12,

  GT511C1R_Command_GetEnrollCount                 = 0x20,
  GT511C1R_Command_CheckEnrolled                  = 0x21,
  GT511C1R_Command_StartEnroll                    = 0x22,
  GT511C1R_Command_EnrollStep1                    = 0x23,
  GT511C1R_Command_EnrollStep2                    = 0x24,
  GT511C1R_Command_EnrollStep3                    = 0x25,
  GT511C1R_Command_HaveFinger                     = 0x26,

  GT511C1R_Command_DeleteOne                      = 0x40,
  GT511C1R_Command_DeleteAll                      = 0x41,

  GT511C1R_Command_VerifyOne                      = 0x50,
  GT511C1R_Command_VerifyAny                      = 0x51,
  GT511C1R_Command_VerifyOneTemplate              = 0x52,
  GT511C1R_Command_VerifyAnyTemplate              = 0x53,

  GT511C1R_Command_CaptureFinger                  = 0x60,
  GT511C1R_Command_MakeTemplateFromCapturedFinger = 0x61,
  GT511C1R_Command_GetImage                       = 0x62,
  GT511C1R_Command_GetRawImage                    = 0x63,

  GT511C1R_Command_GetTemplate                    = 0x70,
  GT511C1R_Command_SetTemplate                    = 0x71,

  GT511C1R_Command_Ack                            = 0x30,
  GT511C1R_Command_NotAck                         = 0x31
} GT511C1R_Command;

typedef enum GT511C1R_ErrorCode
{
  GT511C1R_ErrorCode_Success          = 0,
  GT511C1R_ErrorCode_UARTError        = 0x0801,
  GT511C1R_ErrorCode_BadDevice        = 0x0802,
  GT511C1R_ErrorCode_IDOutOfRange     = 0x1003,
  GT511C1R_ErrorCode_IDNotUsed        = 0x1004,
  GT511C1R_ErrorCode_IDUsed           = 0x1005,
  GT511C1R_ErrorCode_CommError        = 0x1006,
  GT511C1R_ErrorCode_VerifyOneFailed  = 0x1007,
  GT511C1R_ErrorCode_VerifyAnyFailed  = 0x1008,
  GT511C1R_ErrorCode_DBFull           = 0x1009,
  GT511C1R_ErrorCode_DBEmpty          = 0x100A,
  GT511C1R_ErrorCode_BadFinger        = 0x100C,
  GT511C1R_ErrorCode_EnrollFailure    = 0x100D,
  GT511C1R_ErrorCode_BadCommand       = 0x100E,
  GT511C1R_ErrorCode_DeviceError      = 0x100F,
  GT511C1R_ErrorCode_InvalidParam     = 0x1011,
  GT511C1R_ErrorCode_NoFinger         = 0x1012
} GT511C1R_ErrorCode;

typedef struct GT511C1R_CommandPacket_t
{
  uint8_t   SOF1,
            SOF2;
  uint16_t  DeviceID;
  uint32_t  Parameter;
  uint16_t  CommandCode;
  uint16_t  Checksum;
} GT511C1R_CommandPacket_t;

typedef struct GT511C1R_Device_t
{
  void*     UART_Endpoint;
  uint32_t  IsOpen;
  SemaphoreHandle_t Lock;
} GT511C1R_Device_t;

typedef struct GT511C1R_Template_t
{
  uint8_t   Data[506];
} GT511C1R_Template_t;

typedef struct GT511C1R_FPImage_t
{
  uint8_t   Data[51840];
} GT511C1R_FPImage_t;

/**
  Setup the GT511C1R_Device_t object.
**/
void GT511C1R_Init(GT511C1R_Device_t*, void*);

/**
  Close the GT511C1R_Device_t object.
**/
void GT511C1R_DeInit(GT511C1R_Device_t*);

/**
  Low level method to send a specified command and parameter.
**/
uint32_t GT511C1R_SendCommand(GT511C1R_Device_t*, GT511C1R_Command, uint32_t, uint8_t*, uint32_t);

/**
  Initialize the connection to the device.
**/
uint32_t GT511C1R_Open(GT511C1R_Device_t*);

/**
  Close the connection to the device.
**/
uint32_t GT511C1R_Close(GT511C1R_Device_t*);

/**
  Check if the device is open and ready for communication.
**/
uint32_t GT511C1R_IsOpen(GT511C1R_Device_t*);

/**
  Turn on or off the LED on the fingerprint reader.
**/
uint32_t GT511C1R_LED(GT511C1R_Device_t*, uint32_t);

/**
  Change the communication rate.
**/
uint32_t GT511C1R_ChangeUARTBaud(GT511C1R_Device_t*, uint32_t);

/**
  Get the number of stored fingerprint templates.
**/
uint32_t GT511C1R_GetEnrollCount(GT511C1R_Device_t*);

/**
  Check if the specified stored fingerprint template is already used.
**/
uint32_t GT511C1R_IsIDInUse(GT511C1R_Device_t*, uint32_t);

/**
  Start the process of generating a fingerprint template at the specified
  storage location.
**/
uint32_t GT511C1R_StartEnrollment(GT511C1R_Device_t*, uint32_t);

/**
  Take the first scan for fingerprint template generation.
**/
uint32_t GT511C1R_EnrollmentStep1(GT511C1R_Device_t*);

/**
  Take the second scan for fingerprint template generation.
**/
uint32_t GT511C1R_EnrollmentStep2(GT511C1R_Device_t*);

/**
  Take the final scan for fingerprint template generation.
**/
uint32_t GT511C1R_EnrollmentStep3(GT511C1R_Device_t*);

/**
  Check if a finger is pressed against the fingerprint scanner.
**/
uint32_t GT511C1R_IsFingerDown(GT511C1R_Device_t*);

/**
  Delete a stored fingerprint template.
**/
uint32_t GT511C1R_DeleteOneTemplate(GT511C1R_Device_t*, uint32_t);

/**
  Delete all the stored fingerprint template.
**/
uint32_t GT511C1R_DeleteAllTemplates(GT511C1R_Device_t*);

/**
  Verify a pressed finger against a specific stored template.
**/
uint32_t GT511C1R_VerifyOne(GT511C1R_Device_t*, uint32_t);

/**
  Verify a pressed finger against any stored template.
**/
uint32_t GT511C1R_VerifyAny(GT511C1R_Device_t*);

/**
  Verify a specified template against the stored template at the specified
  location.
**/
uint32_t GT511C1R_VerifyOneStoredTemplate(GT511C1R_Device_t*, uint32_t, GT511C1R_Template_t*);

/**
  Verify a specified template against any stored template.
**/
uint32_t GT511C1R_VerifyAnyStoredTemplate(GT511C1R_Device_t*, GT511C1R_Template_t*);

/**
  Capture an image of a pressed finger at the specified quality.
**/
uint32_t GT511C1R_CaptureFingerprint(GT511C1R_Device_t*, uint32_t);

/**
  Get the template for the fingerprint captured by @link GT511C1R_CaptureFingerprint.
**/
uint32_t GT511C1R_GetCapturedFingerprintTemplate(GT511C1R_Device_t*, GT511C1R_Template_t*);

// These methods are not implemented as they take an enormous amount of RAM (~50kB)

/**
  Get the processed image of a fingerprint.
**/
//uint32_t GT511C1R_GetFingerprintImage(GT511C1R_Device_t*, GT511C1R_FPImage_t*);

/**
  Get the raw image of a fingerprint.
**/
//uint32_t GT511C1R_GetRawFingerprintImage(GT511C1R_Device_t*, GT511C1R_FPImage_t*);

/**
  Get the fingerprint template at the specified location.
**/
uint32_t GT511C1R_GetStoredTemplate(GT511C1R_Device_t*, uint32_t, GT511C1R_Template_t*);

/**
  Set the fingerprint template at the specified location.
**/
uint32_t GT511C1R_SetStoredTemplate(GT511C1R_Device_t*, uint32_t, GT511C1R_Template_t*);

#endif // _GT511_GT511C1R_H_
