#ifndef UTILS_H
#define UTILS_H

#include "definitions.h" // Untuk MAX_USERNAME_LEN, MAX_RLE_CODE_LEN

bool run_length_encode(const char* input, char* rle_code, int rle_code_max_len);

#endif // UTILS_H