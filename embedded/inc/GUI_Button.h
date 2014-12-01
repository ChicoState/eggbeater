#ifndef _EGGBEATER_EMBED_GUI_BUTTON_H_
#define _EGGBEATER_EMBED_GUI_BUTTON_H_

#include "common.h"

#include <stm32f4xx.h>

#include <stm32f429i_discovery_lcd.h>
#include <stm32f429i_discovery_ts.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GUI_Button_t
{
  uint32_t  XPosition,
            YPosition,
            Height,
            Width,
            BorderWidth,
            BorderColor,
            FillColor,
            TextColor;
  uint8_t*  Text;
} GUI_Button_t;

typedef enum GUI_Button_Error_t
{
  GUI_Button_Error_Success      = 0,
  GUI_Button_Error_Yes          = 0,
  GUI_Button_Error_Fail         = 1,
  GUI_Button_Error_No           = 1,
  GUI_Button_Error_NullArgument = 0x0801,
  GUI_Button_Error_BadSize      = 0x0802
} GUI_Button_Error_t;

uint32_t gui_button_init(GUI_Button_t*);
uint32_t gui_button_draw(GUI_Button_t*);
uint32_t gui_button_check_touch(GUI_Button_t*, TS_StateTypeDef*);

#ifdef __cplusplus
};
#endif

#endif // _EGGBEATER_EMBED_GUI_BUTTON_H_
