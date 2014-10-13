#include "Keypad.h"

#include <string.h>

#include <stm32f429i_discovery_ts.h>

#define LINE_1_TEXT " 0  1  2  3  4"
#define LINE_2_TEXT "5  6  7  8  9 "

#define LINE_TEXT_LEN 14
#define DEBOUNCE_TIME  2

char kp_getTouchValue(keypad_t*, TS_StateTypeDef*);

int keypad_init(keypad_t* kp, sFONT* font, uint32_t xPos, uint32_t yPos)
{
  if (kp == NULL)
    return -1;

  if (font == NULL)
    return -2;

  if (xPos > BSP_LCD_GetXSize())
    return -3;

  if (yPos > BSP_LCD_GetXSize())
    return -4;

  memset(kp, 0, sizeof(keypad_t));

  kp->xPos = xPos;
  kp->yPos = yPos;

  kp->font = font;
  kp->shouldDraw = 1;

  return 1;
}

int keypad_draw(keypad_t* kp)
{
  sFONT* lastFont = BSP_LCD_GetFont();

  if (kp == NULL)
    return -1;

  if (kp->font == NULL)
    return -2;

  if (kp->xPos > BSP_LCD_GetXSize())
    return -3;

  if (kp->yPos > BSP_LCD_GetXSize())
    return -4;

  BSP_LCD_SetFont(kp->font);


  BSP_LCD_DisplayStringAt(kp->xPos, kp->yPos, (uint8_t*)LINE_1_TEXT, LEFT_MODE);
  BSP_LCD_DisplayStringAt(kp->xPos, kp->yPos + kp->font->Height + 25, (uint8_t*)LINE_2_TEXT, LEFT_MODE);

  kp->shouldDraw = 0;

  BSP_LCD_SetFont(lastFont);

  return 1;
}

int keypad_checktouch(keypad_t* kp)
{
  static uint32_t debounceFlag = 0;
  static uint32_t debounceCounter = DEBOUNCE_TIME;
  TS_StateTypeDef tsState;
  char value = 0;

  if (kp == NULL)
    return -1;

  if (kp->font == NULL)
    return -2;

  if (kp->xPos > BSP_LCD_GetXSize())
    return -3;

  if (kp->yPos > BSP_LCD_GetXSize())
    return -4;

  BSP_TS_GetState(&tsState);

  // Debounce code
  if (debounceFlag)
  {
    if (!tsState.TouchDetected)
      debounceCounter--;
    if (debounceCounter == 0)
    {
      debounceFlag = 0;
      debounceCounter = DEBOUNCE_TIME;
    }
  }
  // Touch code
  else if (tsState.TouchDetected)
  {
    // Is the touch on the correct Y range?
    if ((tsState.Y >= kp->yPos) && (tsState.Y <= (kp->yPos + (2 * kp->font->Height) + 25)))
    {
      // What about the X range?
      if ((tsState.X >= kp->xPos) && (tsState.X <= (kp->xPos + (LINE_TEXT_LEN * kp->font->Width))))
      {
        value = kp_getTouchValue(kp, &tsState);
        if ((value > 0) && (kp->bufIndex < KEYPAD_MAX_LEN))
          kp->buffer[kp->bufIndex++] = value;
        debounceFlag = 1;
      }
    }
  }

  return 1;
}

char kp_getTouchValue(keypad_t* kp, TS_StateTypeDef* ts)
{
  if (kp == NULL)
    return -1;

  if (ts == NULL)
    return -2;

  // In first row?
  if (ts->Y < (kp->yPos + 25 + kp->font->Height))
  {
    if (ts->X < (kp->xPos + (3 * kp->font->Width)))
      return '0';
    else if (ts->X < (kp->xPos + (6 * kp->font->Width)))
      return '1';
    else if (ts->X < (kp->xPos + (9 * kp->font->Width)))
      return '2';
    else if (ts->X < (kp->xPos + (12 * kp->font->Width)))
      return '3';
    else if (ts->X < (kp->xPos + (15 * kp->font->Width)))
      return '4';
  }
  // Second row
  else if ((ts->Y > (kp->yPos + 25 + kp->font->Height)))
  {
    if (ts->X < (kp->xPos + (2 * kp->font->Width)))
      return '5';
    else if (ts->X < (kp->xPos + (5 * kp->font->Width)))
      return '6';
    else if (ts->X < (kp->xPos + (8 * kp->font->Width)))
      return '7';
    else if (ts->X < (kp->xPos + (11 * kp->font->Width)))
      return '8';
    else if (ts->X < (kp->xPos + (14 * kp->font->Width)))
      return '9';
  }

  return 0;
}
