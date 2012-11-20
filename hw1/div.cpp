#include "hw1_include.h"

static uint32_t get_width(uint32_t n1, uint32_t n2)
{
    char line[128] = {'\0'};
    snprintf(line, sizeof(line), "%u | %u", n2, n1);
    return strlen(line);
}

static uint32_t get_height(uint32_t n1, uint32_t n2)
{
    uint32_t quotient = n1 / n2;
    char qstr[128] = {'\0'};
    snprintf(qstr, sizeof(qstr), "%u", quotient);
    int n_op = strlen(qstr);

    return n_op*3 + 3; 
}

void div(uint32_t n1, uint32_t n2)
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

    uint32_t row = 0;

    //quocient
    uint32_t quo = n1 / n2;
    put_in_num(array[row++], width, quo, 0);

    //  /------
    uint32_t dash_len = get_num_len(n1) + 1;
    put_in_dash(array[row++], width, dash_len);
    array[1][width - dash_len -1] = '/';

    //longest line
    char line[128] = {'\0'};
    snprintf(line, sizeof(line), "%u | %u", n2, n1);
    strncpy(array[row++], line, width);


    char n1str[128] = {'\0'};
    snprintf(n1str, sizeof(n1str), "%u", n1);
    uint32_t n1len = strlen(n1str);
    uint32_t pos = 0;    //pos of char in n1str, from left to right
    char tmp[128];
    memset(tmp, '\0', sizeof(tmp));  //need to set all space to null
    uint32_t tmpnum = 0;
    uint32_t rem = 0;
    uint32_t qot = 0;

    //find the first output of quocient, different from the remains
    for(pos = 0; pos<n1len; pos++)
    {
        strncpy(tmp, n1str, pos+1);
        tmpnum = strtoul(tmp, NULL, 0);

		//tmpnum == n2 also ok, otherwise ,if n1 < n2, pos == n1len-1 will be the last
        if(tmpnum >= n2 || pos == n1len-1)
        {
            qot = tmpnum / n2;
            rem = tmpnum % n2;
            put_in_num(array[row++], width, n2*qot, n1len-pos-1);
            break;
        }
    }

    dash_len = n1len;
    //then begin the loop
    for(; pos<n1len; pos++)
    {
        //the dash
        put_in_dash(array[row++], width, dash_len--);

        //put the remainder and the next char in n1str
		uint32_t len = 0;
		//only if rem !=0 and is not the last round
		if(rem != 0 || (rem ==0 && pos == n1len-1)) 
		{
        	snprintf(tmp, sizeof(tmp), "%u", rem);
        	len = strlen(tmp);
		}
		if(pos == n1len - 1)  //last round, no next char, just rem
		{
			uint32_t offset = width-len;
			strncpy(array[row++]+offset, tmp, width);
			break;
		}
		tmp[len] = n1str[pos+1];
		tmp[len+1] = '\0';
        tmpnum = strtoul(tmp, NULL, 0);
        put_in_num(array[row++], width, tmpnum, n1len-pos-2);

        //decide the next line
        qot = tmpnum / n2;
        put_in_num(array[row++], width, n2*qot, n1len-pos-2);
        rem = tmpnum % n2;
    }

    //output
    out_put_array(array, width, height);

    delete[] array[0];
    delete[] array;
}
