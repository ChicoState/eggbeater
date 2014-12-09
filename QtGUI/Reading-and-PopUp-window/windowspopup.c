#include <windows.h>

void open_Popup(const char *Error message, const char *message_title)

{
	printf("Open Window Popup on Windows \n");
	MessageBox(0, Error message, message_title, MB_OK);
}
