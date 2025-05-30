#include "utils.h"
#include <string.h>
#include <stdio.h>

bool run_length_encode(const char* input, char* rle_code, int rle_code_max_len) {
    if (input == NULL || rle_code == NULL || strlen(input) == 0) {
        if (rle_code != NULL && rle_code_max_len > 0) rle_code[0] = '\0';
        return true;
    }

    int input_len = strlen(input);
    int rle_idx = 0;

    for (int i = 0; i < input_len; i++) {
        int count = 1;
        while (i + 1 < input_len && input[i] == input[i + 1]) {
            count++;
            i++;
        }

        if (count > 1) {
            int chars_written = snprintf(rle_code + rle_idx, rle_code_max_len - rle_idx, "%d%c", count, input[i]);
            if (chars_written < 0 || rle_idx + chars_written >= rle_code_max_len -1) { 
                rle_code[rle_idx] = '\0';
                return false;
            }
            rle_idx += chars_written;
        } else {
            if (rle_idx + 1 >= rle_code_max_len -1) { 
                rle_code[rle_idx] = '\0';
                return false;
            }
            rle_code[rle_idx++] = input[i];
        }
    }
    rle_code[rle_idx] = '\0';
    return true;
}