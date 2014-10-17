#include "keypad.h"

#include <string.h>

#include <stm32f429i_discovery_ts.h>

#define LINE_1_TEXT "0  1  2  3  4"
#define LINE_2_TEXT "5  6  7  8  9"

#define LINE_PADDING  25
#define LINE_TEXT_LEN 12
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
  BSP_LCD_DisplayStringAt(kp->xPos, kp->yPos + kp->font->Height + LINE_PADDING, (uint8_t*)LINE_2_TEXT, LEFT_MODE);

  kp->shouldDraw = 0;

  BSP_LCD_SetFont(lastFont);

  return 1;
}

int keypad_checktouch(keypad_t* kp, TS_StateTypeDef* ts)
{
  static uint32_t debounceFlag = 0;
  static uint32_t debounceCounter = DEBOUNCE_TIME;
  //TS_StateTypeDef tsState;
  char value = 0;

  if (kp == NULL)
    return -1;

  if (kp->font == NULL)
    return -2;

  if (kp->xPos > BSP_LCD_GetXSize())
    return -3;

  if (kp->yPos > BSP_LCD_GetXSize())
    return -4;

  //BSP_TS_GetState(&tsState);

  // Debounce code
  if (debounceFlag)
  {
    if (!ts->TouchDetected)
      debounceCounter--;
    if (debounceCounter == 0)
    {
      debounceFlag = 0;
      debounceCounter = DEBOUNCE_TIME;
    }
  }
  // Touch code
  else if (ts->TouchDetected)
  {
    // Is the touch on the correct Y range?
    if ((ts->Y >= kp->yPos) && (ts->Y <= (kp->yPos + (2 * kp->font->Height) + LINE_PADDING)))
    {
      // What about the X range?
      if ((ts->X >= kp->xPos) && (ts->X <= (kp->xPos + (LINE_TEXT_LEN * kp->font->Width))))
      {
        value = kp_getTouchValue(kp, ts);
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
  if (ts->Y < (kp->yPos + (LINE_PADDING / 2) + kp->font->Height))
  {
    if (ts->X < (kp->xPos + (2 * kp->font->Width)))
      return '0';
    else if (ts->X < (kp->xPos + (5 * kp->font->Width)))
      return '1';
    else if (ts->X < (kp->xPos + (8 * kp->font->Width)))
      return '2';
    else if (ts->X < (kp->xPos + (11 * kp->font->Width)))
      return '3';
    else if (ts->X < (kp->xPos + (14 * kp->font->Width)))
      return '4';
  }
  // Second row?
  else if ((ts->Y >= (kp->yPos + (LINE_PADDING / 2) + kp->font->Height)))
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

int backspace_init(backspace_t* bs, uint32_t xPos, uint32_t yPos, uint32_t height, uint32_t width, uint32_t borderWidth, uint32_t borderColor, uint32_t fillColor)
{
  if (bs == NULL)
    return -1;

  if (xPos > BSP_LCD_GetXSize())
    return -2;

  if (yPos > BSP_LCD_GetXSize())
    return -3;

  if ((xPos + width) > BSP_LCD_GetXSize())
    return -4;

  if ((yPos + height) > BSP_LCD_GetYSize())
    return -5;

  if (borderWidth > width)
    return -6;

  if (borderWidth > height)
    return -7;

  bs->xPos        = xPos;
  bs->yPos        = yPos;
  bs->height      = height;
  bs->width       = width;
  bs->borderWidth = borderWidth;
  bs->borderColor = borderColor;
  bs->fillColor   = fillColor;

  return 0;
}

int backspace_draw(backspace_t* bs)
{
  uint32_t lastColor = BSP_LCD_GetTextColor();

  if (bs == NULL)
    return -1;

  if (bs->xPos > BSP_LCD_GetXSize())
    return -2;

  if (bs->yPos > BSP_LCD_GetXSize())
    return -3;

  if ((bs->xPos + bs->width) > BSP_LCD_GetXSize())
    return -4;

  if ((bs->yPos + bs->height) > BSP_LCD_GetYSize())
    return -5;

  if (bs->borderWidth > bs->width)
    return -6;

  if (bs->borderWidth > bs->height)
    return -7;

  BSP_LCD_SetTextColor(bs->borderColor);

  BSP_LCD_FillRect(bs->xPos, bs->yPos, bs->width, bs->height);

  BSP_LCD_SetTextColor(bs->fillColor);

  BSP_LCD_FillRect(bs->xPos + bs->borderWidth, bs->yPos + bs->borderWidth, bs->width - (2 * bs->borderWidth), bs->height - (2 * bs->borderWidth));

  BSP_LCD_SetTextColor(lastColor);

  return 0;
}

int backspace_checktouch(backspace_t* bs, TS_StateTypeDef* ts)
{
  static uint32_t debounceFlag = 0;
  static uint32_t debounceCounter = DEBOUNCE_TIME;
  uint32_t  left,
            right,
            top,
            bottom;
  if (bs == NULL)
    return -1;

  if (bs->xPos > BSP_LCD_GetXSize())
    return -2;

  if (bs->yPos > BSP_LCD_GetXSize())
    return -3;

  if ((bs->xPos + bs->width) > BSP_LCD_GetXSize())
    return -4;

  if ((bs->yPos + bs->height) > BSP_LCD_GetYSize())
    return -5;

  left    = bs->xPos;
  right   = bs->xPos + bs->width;
  top     = bs->yPos;
  bottom  = bs->yPos + bs->height;

  if (debounceFlag)
  {
    if (!ts->TouchDetected)
      debounceCounter--;

    if (debounceCounter == 0)
    {
      debounceFlag    = 0;
      debounceCounter = DEBOUNCE_TIME;
    }
  }
  else
  {
    if (ts->TouchDetected)
    {
      if (ts->X >= left && ts->X <= right && ts->Y >= top && ts->Y <= bottom)
      {
        debounceFlag = 1;
        return 1;
      }
      else
        return 0;
    }
  }

  return 0;
}

void Keypad_Task(void* arg)
{
  uint32_t nextDrawUpdate = 0;
  TickType_t lastWake = 0;
  keypad_t kp;
  backspace_t bs;
  TS_StateTypeDef ts;

  UNUSED_ARG(arg);

  InitLCD();

  keypad_init(&kp, &Font16, 20, 230);
  backspace_init(&bs, 174, 230, 55, 57, 3, LCD_COLOR_BLACK, LCD_COLOR_RED);

  vTaskDelay(50);

  keypad_draw(&kp);
  backspace_draw(&bs);

  while (1)
  {
    BSP_TS_GetState(&ts);
//    keypad_draw(&kp);
//    backspace_draw(&bs);
    keypad_checktouch(&kp, &ts);

    if (backspace_checktouch(&bs, &ts) > 0)
    {
      if (kp.bufIndex > 0)
      {
        kp.bufIndex--;

        kp.buffer[kp.bufIndex] = 0;
      }
    }

    if (HAL_GetTick() > nextDrawUpdate)
    {
      keypad_draw(&kp);
      backspace_draw(&bs);

      BSP_LCD_ClearStringLine(3);
      BSP_LCD_DisplayStringAtLine(3, (uint8_t*)kp.buffer);

      nextDrawUpdate += 20;
    }

    vTaskDelayUntil(&lastWake, 10);
  }
}
