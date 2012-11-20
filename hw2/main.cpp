#include "hw2_include.h"
#include "trans.hpp"
#include "disp.hpp"

void print_help()
{
    cerr << "malformed command.\n";
    cerr << "Usage:\n";
    cerr << "hw2 sort [tfile]\n";
    cerr << "hw2 display topdir\n";
}

void run_sort_stdin()
{
    TransRecord trans(cin);
    trans.doRun();
}

int check_file(const char *file)
{
    if (file[0] == '-') {
        print_help();
        return FALSE;
    }

    if (-1 == open(file, O_RDONLY)) {
        switch (errno) {
            case EACCES:
                cerr << "Input file \"" << file << "\" access denied.\n";
                break;
            case ENOENT:
                cerr << "Input file \"" << file << "\" not exist.\n";
                break;
            default:
                cerr << "Input file \"" << file << "\" open error.\n";
                break;
        }
        return FALSE;
    }

    return TRUE;
}

int check_trans_file(const char *file) 
{
    if (FALSE == check_file(file)) {
        return FALSE;
    }

    struct stat fs;
    stat(file, &fs);
    if (S_ISDIR(fs.st_mode)) {
        cerr << "Input file \"" << file << "\" is a directory.\n";
        return FALSE;
    }

    return TRUE;
}

int check_disp_file(const char *file) 
{
    if (FALSE == check_file(file)) {
        return FALSE;
    }

    struct stat fs;
    stat(file, &fs);
    if (S_ISREG(fs.st_mode)) {
        cerr << "Input file \"" << file << "\" is not a directory.\n";
        return FALSE;
    }

    return TRUE;
}

void run_sort_file(const char *file)
{
    if (FALSE == check_trans_file(file)) {
        return;
    }
    ifstream fin(file);
    TransRecord trans(fin);
    trans.doRun();
}

void run_display(const char *dir)
{
    if (FALSE == check_disp_file(dir)) {
        return;
    }
    char name[MAX_FILE_NAME_LEN] = {'\0'};
    strncpy(name, dir, sizeof(name));
    int len = strlen(dir);
    if (name[len-1] == '/' && len != 1) {
        name[len-1] = '\0';
    }
    struct stat st;
    stat(name, &st);
    Disp disp(name, name, st.st_mode, 0);
    disp.doRun();
   // cout << "after dorun\n";
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        print_help();
        return -1;
    }

    if (strcmp(argv[1], "sort") == 0) {
        if (argc == 2) {
            run_sort_stdin();
        } else if (argc == 3) {
            run_sort_file(argv[2]);
        } else {
            print_help();
        }
    
    } else if (strcmp(argv[1], "display") == 0) {
        if (argc == 3) {
            run_display(argv[2]);
        } else {
            print_help();
        }
    } else {
        print_help();
    }

    return 0;
}
