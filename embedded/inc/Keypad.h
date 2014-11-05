#ifndef _EGGBEATER_KEYPAD_H_
#define _EGGBEATER_KEYPAD_H_

#include <stdint.h>
#include <stm32f4xx.h>
#include <stm32f429i_discovery_lcd.h>
#include <stm32f429i_discovery_ts.h>

#include "common.h"

#define KEYPAD_MAX_LEN 16

typedef struct keypad_t
{
  uint32_t  xPos,
            yPos;
  char      buffer[KEYPAD_MAX_LEN+1];
  uint32_t  bufIndex;
  uint32_t  lastTouch;
  sFONT*    font;
  volatile uint32_t shouldDraw,
                    haveTouch;
} keypad_t;

typedef struct backspace_t
{
  uint32_t  xPos,
            yPos,
            height,
            width,
            borderWidth,
            borderColor,
            fillColor;
} backspace_t;

int keypad_init(keypad_t*,  sFONT*, uint32_t, uint32_t);

int keypad_draw(keypad_t*);

int keypad_checktouch(keypad_t*, TS_StateTypeDef*);


int backspace_init(backspace_t*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

int backspace_draw(backspace_t*);

int backspace_checktouch(backspace_t*, TS_StateTypeDef*);

// FreeRTOS task entry point
void Keypad_Task(void*);

#endif // _EGGBEATER_KEYPAD_H_
