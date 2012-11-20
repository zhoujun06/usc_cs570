#include "hw1_include.h"


int check_param(int argc, char * const argv[], uint32_t &n1, uint32_t &n2)
{
    if(argc < 4)
    {
        cerr << "Error:\r\n\tmalformed command" << endl;
        return -1;
    }

    if((strcmp(argv[1], "mul") != 0) && (strcmp(argv[1], "div") != 0))
    {
        cerr << "Error:\r\n\tmalformed command" << endl;
        return -1;
    }

    if(argc > 4)
    {
        cerr << "Error:\r\n\ttoo many arguments" << endl;
        return -1;
    }

    if(argv[2][0] == '-') 
    {
        cerr << "Error:\r\n\tn1 must be unsigned interger." << endl;
        return -1;
    }

    if(argv[3][0] == '-') 
    {
        cerr << "Error:\r\n\tn2 must be unsigned interger." << endl;
        return -1;
    }

    char *pend;
    n1 = strtoul(argv[2], &pend, 10);

    if(errno == ERANGE)
    {
        cerr << "Error:\r\n\tn1 is out of range." << endl;
        return -1;
    }
    if((*pend) != '\0')
    {
        cerr << "Error:\r\n\tn1 is malformed." << endl;
        return -1;
    }

    n2 = strtoul(argv[3], &pend, 10);

    if(errno == ERANGE)
    {
        cerr << "Error:\r\n\tn2 is out of range." << endl;
        return -1;
    }
    if((*pend) != '\0')
    {
        cerr << "Error:\r\n\tn2 is malformed." << endl;
        return -1;
    }

    if(n2 == 0 && strcmp(argv[1], "div") == 0)
    {
        cerr << "Error:\r\n\tn2 is invalid." << endl;
        return -1;
    }
    
    return 0;
}

void print_help(void)
{
    cerr << "usage:" << endl;
    cerr << "\thw1 mul n1 n2" << endl;
    cerr << "\thw1 div n1 n2" << endl << endl;
	cerr << "\tmul: perform long-hand integer multiplication - multiply n1 by n2, where n1 and n2" ;
	cerr << "tare unsigned 32-bit integers (i.e., with a maximum value of 4294967295). n1 will be" ;
	cerr << "treferred to as the multiplicand and n2 will be referred to as the multiplier." << endl ;
	cerr << "\tdiv: perform long-hand integer division - divide n1 by n2 to produce a quotient and" ;
	cerr << "ta remainder. n1 and n2 are unsigned 32-bit integers (i.e., with a maximum value " ;
	cerr << "tof 4294967295)." << endl;
    
    /*
	cerr << "\tmul: perform long-hand integer multiplication - multiply n1 by n2, where n1 and n2" << endl;
	cerr << "\tare unsigned 32-bit integers (i.e., with a maximum value of 4294967295). n1 will be" << endl;
	cerr << "\treferred to as the multiplicand and n2 will be referred to as the multiplier." << endl << endl;
	cerr << "\tdiv: perform long-hand integer division - divide n1 by n2 to produce a quotient and" << endl;
	cerr << "\ta remainder. n1 and n2 are unsigned 32-bit integers (i.e., with a maximum value " << endl;
	cerr << "\tof 4294967295)." << endl;
    */
    cerr << endl;
}

int main(int argc, char *argv[])
{
    uint32_t n1 = 0;
    uint32_t n2 = 0;
    int ret = 0;

    if((ret = check_param(argc, argv, n1, n2)) < 0)
    {
        print_help();
        return -1;
    }

    if(strcmp(argv[1], "mul") == 0)
    {
        mul(n1, n2);
    }
    else 
    {
        div(n1, n2);
    }

    return 0;
}
