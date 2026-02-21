#pragma once

#define HAVE_STDINT_H
#define HAVE_POSIX_MEMALIGN
#include "block_base.h"

extern "C"
{
struct Blowfish_State;
struct CbcModeState;

int Blowfish_start_operation(const uint8_t key[], size_t key_len, Blowfish_State **pResult);

int Blowfish_stop_operation(Blowfish_State *arg);

int CBC_start_operation(BlockBase *cipher, const uint8_t iv[],
	size_t iv_len, CbcModeState **pResult);

int CBC_stop_operation(CbcModeState *state);

int CBC_decrypt(CbcModeState *cbcState, const uint8_t *in,
	uint8_t *out, size_t data_len);
}
