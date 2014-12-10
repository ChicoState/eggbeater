#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "popup.h"
#ifdef _WIN64
#	include "getline.c"
#elif _WIN32
#	include "getline.c"
#endif

int is_substring(const char *p_string, const char *c_string)
{
	char *substring;
	int result = 0;
	substring = strstr(p_string, c_string);
	if(substring != NULL)
		result = 1;
	return result;
}

int main(void)
{
	FILE *fp;
	char *line = NULL, Error message[BUFSIZ], *message_title;
	size_t len = 0;
	int line_n = 0;
	char error_str[12] = "Fatal", warning_str[12] = "simple";

	fp = fopen("data.txt", "r");
	if (fp == NULL)
		exit(0);

	while (getline(&line, &len, fp) != -1) {
		line_n++;
		if(is_substring(line,error_str) == 1)
		{
			message_title = "Error";
			sprintf(Error message, "At %d line", line_n);
			open_Popup(Error message, message_title);
			break;
		}
		else if (is_substring(line,warning_str) == 1) 
		{
			message_title = "Warning";
			sprintf(Error message, "At %d line", line_n);
			open_Popup(Error message, message_title);
		}
		printf("Line - %d: %s", line_n, line);
	}

	free(line);
	fclose(fp);
	exit(1);
}
