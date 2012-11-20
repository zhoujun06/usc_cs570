#include "hw1_include.h"

static uint32_t get_width(const uint32_t n1, const uint32_t n2)
{
    char n1str[100] = {'\0'};
    char n2str[100] = {'\0'};
    char n3str[100] = {'\0'};
 
    uint64_t n3 = (uint64_t)n1 * (uint64_t)n2;

    snprintf(n1str, sizeof(n1str),"%u", n1);
    snprintf(n2str, sizeof(n2str),"%u", n2);
    snprintf(n3str, sizeof(n3str),"%lu", n3);

    int l1 = strlen(n1str);
    int l2 = strlen(n2str);
    int l3 = strlen(n3str);

    if(l1 > l2 + 2)
    {
        return l1 > l3 ? l1 : l3;
    } 
    else 
    {
        return l3 > l2 + 2 ? l3 : l2 + 2;
    }
}

static uint32_t get_height(const uint32_t n1, const uint32_t n2)
{
    if(n1 ==0 || n2 == 0)
        return 4;

    char n2str[100] = {'\0'};
    snprintf(n2str, sizeof(n2str),"%u", n2);
    if(n2 < 10)
        return 4;
    else 
        return strlen(n2str) + 5;
}


void mul(uint32_t n1, uint32_t n2)
{
    uint32_t width = get_width(n1, n2);
    uint32_t height = get_height(n1, n2);

    char **array = new char *[height];
    if(NULL == array)
    {
        cerr << "memory allocate failed." << endl;
        exit(-1);
    }

    array[0] = new char[width * height];
    if(NULL == array[0])
    {
        cerr << "memory allocate failed." << endl;
        exit(-1);
    }

    for(uint32_t i=1; i<height; i++)
    {
        array[i] = array[i-1] + width;
    }

    memset(array[0], ' ', width * height); 

    int row = 0;        //indicates the number of rows filled
    put_in_num(array[row++], width, n1, 0);      //n1
    array[row][0] = 'x';
    put_in_num(array[row++], width, n2, 0);      //x  n2
    put_in_dash(array[row++], width);          //-----
    
    if(n1 == 0 || n2 == 0)
    {
        put_in_num(array[row], width, 0, 0);    //
        out_put_array(array, width, height);    //
        return;
    }
    
    int pad = 0;        //padding space needed at the end of each row
    for(uint32_t tmp = n2; tmp > 0; tmp /= 10)
    {
        uint64_t n = tmp % 10;
        uint64_t num = (uint64_t)n1 * n;
        put_in_num(array[row++], width, num, pad);
        pad++;
    }

    if(row > 4)     //n2 is bigger than 9, need another two rows, otherwise, done
    {
        put_in_dash(array[row++], width);
        put_in_num(array[row], width, (uint64_t)n1*(uint64_t)n2, 0);
    }

    //output
    out_put_array(array, width, height);    

    //
    delete[] array[0];
    delete[] array;
}
