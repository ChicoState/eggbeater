#ifndef _EGGBEATER_EMBED_FINGERPRINT_READER_H_
#define _EGGBEATER_EMBED_FINGERPRINT_READER_H_

#include "common.h"

void InitUART(void);

uint32_t  fp_session_open();
uint32_t  fp_session_new();

void      fp_session_refresh(uint32_t id);
void      fp_session_clos(uint32_t id);

void      fp_generate_file_key(uint32_t id);

// RTOS task entry point
void Fingerprint_Task(void*);

#endif // _EGGBEATER_EMBED_FINGERPRINT_READER_H_
