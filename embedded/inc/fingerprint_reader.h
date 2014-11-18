#ifndef _EGGBEATER_EMBED_FINGERPRINT_READER_H_
#define _EGGBEATER_EMBED_FINGERPRINT_READER_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void InitUART(void);

uint32_t  fp_session_open(void);
uint32_t  fp_session_new(void);

void      fp_session_refresh(uint32_t id);
void      fp_session_close(uint32_t id);

void      fp_generate_file_key(uint32_t id);

// RTOS task entry point
void Fingerprint_Task(void*);

#ifdef __cplusplus
};
#endif

#endif // _EGGBEATER_EMBED_FINGERPRINT_READER_H_
