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

// Because min is crazy
#define egb_min(a, b) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )

#ifdef __cplusplus
extern "C" {
#endif

void InitLCD(void);

#ifdef __cplusplus
};
#endif

#endif // _EGGBEATER_COMMON_H_
