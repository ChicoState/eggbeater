#include <windows.h>

void open_Popup(const char *message_box, const char *message_title)

{
	printf("Open Window Popup on Windows \n");
	MessageBox(0, message_box, message_title, MB_OK);
}
