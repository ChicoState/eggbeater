#ifndef _EGGBEATER_COMMON_H_
#define _EGGBEATER_COMMON_H_

#include <stdint.h>

#ifndef _EGGBEATER_TESTS_

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#endif

#define LCD_FRAME_BUFFER_LAYER0 ( LCD_FRAME_BUFFER )
#define LCD_FRAME_BUFFER_LAYER1 ( LCD_FRAME_BUFFER + BUFFER_OFFSET )

#ifndef UNUSED_ARG
  #define UNUSED_ARG(x) ( (void) ( x ) )
#endif // UNUSED_ARG

#ifndef egb_min
  // Because min is crazy
  #define egb_min(a, b) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#endif // egb_min

#ifdef __cplusplus
extern "C" {
#endif

// Buffer, length pair used to pass transmission packets between modules
typedef struct Packet_t
{
  uint8_t* Data;
  uint32_t Length;
} Packet_t;

#ifndef _EGGBEATER_TESTS_
// Pair of message queues to synchronize passing of packets between modules
typedef struct RTOS_Data_Queue_t
{
  QueueHandle_t Tx;
  QueueHandle_t Rx;
} RTOS_Data_Queue_t;
#else
typedef struct RTOS_Data_Queue_t
{
  void* Tx,
      * Rx;
} RTOS_Data_Queue_t;
#endif

// Initialize the LCD and touch screen
void InitLCD(void);

#ifdef __cplusplus
};
#endif

#endif // _EGGBEATER_COMMON_H_
