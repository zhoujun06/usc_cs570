#ifndef _DISP_H_
#define _DISP_H_

#include "hw2_include.h"

const int MAX_FILE_NAME_LEN = 256;
class Disp
{
    public:
        Disp(const char *path, const char *name, mode_t type, int level = 0);
        ~Disp();

        int readin();
        int readin(char *dir);
        void output(uint64_t bits);
        void printPre(uint64_t bits);
        void doRun();
    private:
        int insert(Disp *node);
    private:
        int _level;
        int _fail; // if a dir, whether read fail, distinguish between empty
        mode_t _type; // use stat to get it, S_IFDIR, S_IFREG
        char _name[MAX_FILE_NAME_LEN]; //file name, no path, no '/'
        char _path[MAX_FILE_NAME_LEN]; //whole path, include name
        My570List *_child;
};
#endif
