#ifdef _WIN64
#	include "windowspopup.c"
#elif _WIN32
#	include "windowspopup.c"
#else
#	include "gtkpopup.c"
#endif

void open_Popup(const char *Error message, const char *message_title);

