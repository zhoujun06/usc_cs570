#ifndef UTILS_H
#define UTILS_H

uint32_t get_num_len(uint64_t num);
void put_in_num(char *array, uint32_t width, uint64_t num, uint32_t pad);
void put_in_dash(char *array, uint32_t width);
void put_in_dash(char *array, uint32_t width, uint32_t num);
void out_put_array(char **array, uint32_t width, uint32_t height);

#endif
