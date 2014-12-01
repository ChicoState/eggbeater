#include "GUI_Button.h"
#include <string.h>

#define DEBOUNCE_TIME  2

uint32_t gui_button_init(GUI_Button_t* b)
{
  if (b == NULL)
    return GUI_Button_Error_NullArgument;

  b->XPosition    = 0;
  b->YPosition    = 0;
  b->Height       = 10;
  b->Width        = 10;
  b->BorderWidth  = 10;
  b->BorderColor  = LCD_COLOR_BLACK;
  b->FillColor    = LCD_COLOR_WHITE;
  b->TextColor    = LCD_COLOR_BLACK;
  b->Text         = NULL;

  return GUI_Button_Error_Success;
}

uint32_t gui_button_draw(GUI_Button_t* b)
{
  uint32_t lastColor = BSP_LCD_GetTextColor();
  uint32_t strLen = 0;

  if (b == NULL)
    return GUI_Button_Error_NullArgument;

  if (b->XPosition > BSP_LCD_GetXSize())
    return GUI_Button_Error_BadSize;

  if (b->YPosition > BSP_LCD_GetYSize())
    return GUI_Button_Error_BadSize;

  if ((b->XPosition + b->Width) > BSP_LCD_GetXSize())
    return GUI_Button_Error_BadSize;

  if ((b->YPosition + b->Height) > BSP_LCD_GetYSize())
    return GUI_Button_Error_BadSize;

  if (b->BorderWidth > b->Width)
    return GUI_Button_Error_BadSize;

  if (b->BorderWidth > b->Height)
    return GUI_Button_Error_BadSize;

  BSP_LCD_SetTextColor(b->BorderColor);

  BSP_LCD_FillRect(b->XPosition, b->YPosition, b->Width, b->Height);

  BSP_LCD_SetTextColor(b->FillColor);

  BSP_LCD_FillRect(b->XPosition + b->BorderWidth, b->YPosition + b->BorderWidth, b->Width - (2 * b->BorderWidth), b->Height - (2 * b->BorderWidth));

  if (b->Text != NULL)
  {
    strLen = strlen((char*)b->Text);

    BSP_LCD_SetTextColor(b->TextColor);

    //BSP_LCD_DisplayStringAt(b->XPosition + (2 * b->BorderWidth), b->YPosition + (2 * b->BorderWidth), b->Text, CENTER_MODE);
    BSP_LCD_DisplayStringAt(b->XPosition + (b->Width / 2) - (strLen * BSP_LCD_GetFont()->Width / 2), b->YPosition + (b->Height / 2) - (BSP_LCD_GetFont()->Height / 2), b->Text, LEFT_MODE);
  }

  BSP_LCD_SetTextColor(lastColor);

  return GUI_Button_Error_Success;
}

uint32_t gui_button_check_touch(GUI_Button_t* b, TS_StateTypeDef* ts)
{
  static uint32_t debounceFlag = 0;
  static uint32_t debounceCounter = DEBOUNCE_TIME;

  uint32_t  left,
            right,
            top,
            bottom;

  if (b == NULL)
    return GUI_Button_Error_NullArgument;

  if (b->XPosition > BSP_LCD_GetXSize())
    return GUI_Button_Error_BadSize;

  if (b->YPosition > BSP_LCD_GetYSize())
    return GUI_Button_Error_BadSize;

  if ((b->XPosition + b->Width) > BSP_LCD_GetXSize())
    return GUI_Button_Error_BadSize;

  if ((b->YPosition + b->Height) > BSP_LCD_GetYSize())
    return GUI_Button_Error_BadSize;

  left    = b->XPosition;
  right   = b->XPosition + b->Width;
  top     = b->YPosition;
  bottom  = b->YPosition + b->Height;

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
        return GUI_Button_Error_Yes;
      }
      else
        return GUI_Button_Error_No;
    }
  }

  return GUI_Button_Error_No;
}
