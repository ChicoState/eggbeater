#ifndef _EGGBEATER_EMBED_FINGERPRINT_READER_H_
#define _EGGBEATER_EMBED_FINGERPRINT_READER_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum fp_error_t
{
  fp_error_session_not_started  = 0x0900,
  fp_error_id_invalid           = 0x9001,
  fp_session_expired            = 0x9002
} fp_error_t;

void InitUART(void);
void InitRTC(void);

uint32_t  fp_session_open(void);
uint32_t  fp_session_new(void);

uint32_t  fp_session_refresh(uint32_t id);
uint32_t  fp_session_close(uint32_t id);

uint32_t  fp_generate_file_key(uint32_t id, uint8_t* digest);

// RTOS task entry point
void Fingerprint_Task(void*);

#ifdef __cplusplus
};
#endif

#endif // _EGGBEATER_EMBED_FINGERPRINT_READER_H_
