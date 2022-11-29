#include "input_parser.h"

int find_idx( const int a[], size_t size, int value )
{
    size_t index = 0;

    while ( index < size && a[index] != value ) ++index;

    int idx = ( index == size ? -1 : index );

    return idx;
}

int numbers_count(uint8_t* buffer, int count_size, int count_idx){
    int length = 0;
    for (int i = count_idx; i < count_size; i++)
    {
        if (buffer[i] == 'x')
        {
            length += 1;
        }
        
    }
    return length;
}

int parse_input_float(uint8_t* input_arr, int batch_size, int start_idx, float* processed_buffer, int write_idx)
{
    int pos = 0;

    char number[max_number_length];

    batch_size += start_idx;
    for(int i = start_idx; i < batch_size; i++){

        if(input_arr[i] != 'x'){
            number[pos] = input_arr[i];
            pos += 1;
        }
        else if (input_arr[i] == 'x'){
            float num = strtof(number, NULL);
            processed_buffer[write_idx] = num;
            write_idx++;
            pos = 0;
        }
    }

    return write_idx;
}
