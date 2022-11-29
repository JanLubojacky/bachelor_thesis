#ifndef __INPUT_PARSER_H__
#define __INPUT_PARSER_H__

#include <stdio.h>
#include "stdint.h"
#include <string.h>
#include <stdlib.h>
#define max_number_length 20

int find_idx(const int a[], size_t size, int value);

int numbers_count(uint8_t* buffer, int count_size, int count_idx);

int parse_input_float(uint8_t* input_arr, int batch_size, int start_idx, float* processed_buffer, int write_idx);

#endif /* __INPUT_PARSER_H__ */
