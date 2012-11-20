#include "hw1_include.h"

uint32_t get_num_len(uint64_t num)
{
    char str[128] = {'\0'};
    snprintf(str, sizeof(str), "%lu", num);
    return strlen(str);
}

void put_in_num(char *array, uint32_t width, uint64_t num, uint32_t pad)
{
    if(array == NULL || width == 0)
    {
        return;
    }

    char nstr[100] = {'\0'};
    snprintf(nstr, sizeof(nstr), "%lu", num);
    if(strlen(nstr) > width)
    {
        return;
    }

    int offset = (width - pad) - strlen(nstr);
    strncpy(array+offset, nstr, strlen(nstr));

    return;
}

void put_in_dash(char *array, uint32_t width)
{
    if(array == NULL || width == 0)
    {
        return;
    }

    memset(array, '-', width);
    return;
}

void put_in_dash(char *array, uint32_t width, uint32_t num)
{
    if(width < num)
    {
        return;
    }

    uint32_t offset = width - num;
    memset(array+offset, '-', num);
    return;
}

void out_put_array(char **array, uint32_t width, uint32_t height)
{
    for(uint32_t i=0; i<height; i++)
    {
        cout << '\t';
        for(uint32_t j=0; j<width; j++)
        {
            cout << array[i][j];
        }
        cout << endl;
    }
}
