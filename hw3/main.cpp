#include "hw3_include.h"
#include "bst.hpp"

bool g_display_all = false;
bool g_from_std = false;
bool g_is_avl = false;

char * g_input_file = NULL;
std::vector<char *> g_str_vec;

enum cmd_t {
    CMD_BST = 1,
    CMD_BST_DEL,
    CMD_AVL,
    CMD_AVL_DEL,
};

void print_help()
{
    cerr << "malformed command.\n";
    cerr << "Usage:\n";
    cerr << "hw3 bst [-displayall] [file]\n";
    cerr << "hw3 bst_delete [-displayall] string1 string2 ... stringN file\n";
    cerr << "hw3 avl [-displayall] [file]\n";
    cerr << "hw3 avl_delete [-displayall] string1 string2 ... stringN file\n";
    exit(1);
}

void run_bst_stdin()
{
    Bst bst(cin);
    bst.build();
}

int check_file(const char *file)
{
    if (file[0] == '-') {
        print_help();
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

void run_bst_file(const char *file)
{
    if (FALSE == check_file(file)) {
        exit(1);
    }

    ifstream fin(file);
    if (!fin.good()) {
        cerr << "read file: " << file << " failed.\n";
        exit(1);
    }
    Bst bst(fin);
    bst.build();
}

void run_bst_del(char *file, std::vector<char *> & strvec)
{
    if (FALSE == check_file(file)) {
        exit(1);
    }

    ifstream fin(file);
    if (!fin.good()) {
        cerr << "read file: " << file << " failed.\n";
        exit(1);
    }
    Bst bst(fin);
    bst.build();

    for (size_t i=0; i< strvec.size(); i++) {
        cout << "\nDeleted \""<< strvec[i] << "\":\n\n";
        bst.del(strvec[i]);
        bst.display();
    }
}

int main(int argc, char *argv[])
{
    int cmd = 0;

    argc--;
    argv++;

    if (argc <= 0) {
        print_help();
    }

    if (strcmp(*argv, "bst") == 0) {
        cmd = CMD_BST;
    } else if (strcmp(*argv, "bst_delete") == 0) {
        cmd = CMD_BST_DEL;
    } else if (strcmp(*argv, "avl") == 0) {
        cmd = CMD_AVL;
        g_is_avl = true;
    } else if (strcmp(*argv, "avl_delete") == 0) {
        cmd = CMD_AVL_DEL;
        g_is_avl = true;
    } else {
        print_help();
    }

    if (cmd == CMD_BST || cmd == CMD_AVL) {
        argc--;
        argv++;
        if (argc == 0) {
            g_from_std = true;
        } else if (argc == 1) {
            if (strcmp(*argv, "-displayall") == 0) {
                g_display_all = true;
                g_from_std = true;
            } else {
                g_input_file = *argv;
            } 
        } else if (argc == 2) {
            if (strcmp(*argv, "-displayall") == 0) {
                g_display_all = true;
                g_input_file = *(argv+1);
            } else if (strcmp(*(argv+1), "-displayall") == 0) {
                g_display_all = true;
                g_input_file = *argv;
            } else {
                print_help();
            } 
        }

    } else {
        for (argc--, argv++; argc>0; argc--, argv++)
        {
            if (strcmp(*argv, "-displayall") == 0) {
                g_display_all = true;
            } else {
                if (argc == 1) {
                    g_input_file = *argv;
                } else {
                    g_str_vec.push_back(*argv);
                }
            }
        }

        if(g_input_file == NULL || g_str_vec.size() == 0) {
            print_help();
        }
    }

    switch (cmd) {
        case CMD_BST:
        case CMD_AVL:
            if (g_from_std) {
                run_bst_stdin();
            } else {
                run_bst_file(g_input_file);
            }
            break;

        case CMD_BST_DEL:
        case CMD_AVL_DEL:
            run_bst_del(g_input_file, g_str_vec);
            break;

            /*
        case CMD_AVL:
            if (g_from_std) {
                run_avl_stdin();
            } else {
                run_avl_file(g_input_file);
            }
            break;

        case CMD_AVL_DEL:
            run_avl_del(g_input_file, g_str_vec);
            break;
            */
    }

    return 0;
}


