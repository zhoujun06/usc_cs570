#include "hw4_include.h"
#include "maze.hpp"
#include "mazedata.hpp"
#include "queue.hpp"
#include "search.hpp"
#include "problem.hpp"

//#define HW_DEBUG

bool g_info = false;
bool g_from_std = false;

int g_top = 0;
int g_bottom = 0;
int g_left = 0;
int g_right = 0;
int g_height = 0;
int g_width = 0;


char * g_input_file = NULL;

enum cmd_t {
    CMD_MAZE_GEN = 1,
    CMD_MAZE_SOV,
};

void print_help()
{
    cerr << "malformed command.\n";
    cerr << "Usage:\n";
    cerr << "hw4 mazegen w h [-info] [-top=tpos] [-bottom=bpos] [-left=lpos] [-right=rpos] [rndfile]\n";
    cerr << "hw4 mazesolve [file]\n";
    cerr << "w should between 1 and 64, h should between 1 and 32\n";
    exit(1);
}

int check_file(const char *file)
{
    /*  
    if (file[0] == '-') {
        print_help();
    }
    */

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

void gen_maze()
{
    if (g_from_std == false) {
        if (FALSE == check_file(g_input_file)) {
            exit(1);
        }

        FILE * fp = fopen(g_input_file, "rb");
        if (NULL == fp) {
            cerr << "read file: " << g_input_file << " failed.\n";
            exit(1);
        }
        Maze maze(fp, g_height, g_width, g_top, g_bottom, g_left, g_right);
        maze.build();
    } else {
        Maze maze(stdin, g_height, g_width, g_top, g_bottom, g_left, g_right);
        maze.build();
    }
}


void run_search(MazeData &maze)
{

    maze.init();
//    maze.printData();
    //maze.printGraph();

    Problem pb(&maze);

    bool result = false;

    if (maze._type == MAZE_TYPE_2) {
        DijkSearch ds;
        std::vector<State *> ds_path;
        result = ds.doSearch(pb, ds_path);
        if (!result) {
            cerr << "no path available.\n";
            exit(1);
        }

        maze.fillPath(ds_path);
        maze.printData();
        int start = maze._entry[0];
        cout << "Starts in room at row = " << maze._room[start]._row+1;
        cout << " and column = " << maze._room[start]._col+1 << ".\n";
        int end = maze._entry[1];
        cout << "Ends in room at row = " << maze._room[end]._row+1;
        cout << " and column = " << maze._room[end]._col+1 << ".\n";

        size_t last = ds_path.size();
        cout << "Cost of shortest path = " << ds_path[last-1]->_path_cost << ".\n";
    } else {
        BFSearch bs;
        int max_level = 0;
        std::vector<State *> bs_path;
        result = bs.doSearch(pb, bs_path, max_level);
        if (!result) {
            cerr << "no path available.\n";
            exit(1);
        }

        maze.fillPath(bs_path);
        maze.printData();
        int start = maze._entry[0];
        cout << "Starts in room at row = " << maze._room[start]._row+1;
        cout << " and column = " << maze._room[start]._col+1 << ".\n";
        int end = maze._entry[1];
        cout << "Ends in room at row = " << maze._room[end]._row+1;
        cout << " and column = " << maze._room[end]._col+1 << ".\n";

        size_t last = bs_path.size();
        cout << "Cost of shortest path = " << bs_path[last-1]->_path_cost << ".\n";
        cout << "Maximum level = " << max_level << ".\n";
    }
}

void solve_maze()
{
    if (false == g_from_std)
    {
        if (FALSE == check_file(g_input_file)) {
            exit(1);
        }
        ifstream fin(g_input_file);
        MazeData maze(fin);
        run_search(maze);
    } else {
        MazeData maze(std::cin);
        run_search(maze);
    }
}

bool isDigit(char *str)
{
    while (*str != '\0') {
        if (*str < '0' || *str > '9') {
            return false;
        }
        str++;
    }
    return true;
}

void checkParams()
{
    if (
            g_height > 32 || g_height < 1 ||
            g_width > 64 || g_width < 1 ||
            g_top > g_width || g_bottom > g_width ||
            g_left > g_height || g_right > g_height
        )
    {
        print_help();
    }
}

int main(int argc, char *argv[])
{
    int cmd = 0;

    if (argc == 1) {
        print_help();
    }

    if (strcmp(argv[1], "mazegen") == 0) {
        cmd = CMD_MAZE_GEN;
    } else if (strcmp(argv[1], "mazesolve") == 0) {
        cmd = CMD_MAZE_SOV;
    } else {
        print_help();
    }

    if (cmd == CMD_MAZE_SOV) {
        if (argc > 3) {
            print_help();
        } else if (argc == 2) {
            g_from_std = true;
        } else {
            g_input_file = argv[2];
        }

        solve_maze();
    } 
    else
    {
        int c = 0;
        int num = 0;
        while (1) {
            static struct option long_options[] =
            {
                {(char *)"info",    no_argument,       0, 'i'},
                {(char *)"top",     required_argument, 0, 't'},
                {(char *)"bottom",  required_argument, 0, 'b'},
                {(char *)"left",    required_argument, 0, 'l'},
                {(char *)"right",   required_argument, 0, 'r'},
                {0, 0, 0, 0}
            };

            int option_index = 0;
            c = getopt_long_only(argc, argv, "it:b:l:r:", long_options, &option_index);
            if (c == -1) {
                break;
            }

            switch (c) {
                case 'i':
                    g_info = true;
                    break;
                case 't':
                    if (!isDigit(optarg)) {
                        print_help();
                    }
                    g_top = atoi(optarg);
                    if (g_top < 1 || g_top > 64) {
                        print_help();
                    }
                    num++;
                    break;
                case 'b':
                    if (!isDigit(optarg)) {
                        print_help();
                    }
                    g_bottom = atoi(optarg);
                    if (g_bottom < 1 || g_bottom > 64) {
                        print_help();
                    }
                    num++;
                    break;
                case 'l':
                    if (!isDigit(optarg)) {
                        print_help();
                    }
                    g_left = atoi(optarg);
                    if (g_left < 1 || g_left > 32) {
                        print_help();
                    }
                    num++;
                    break;
                case 'r':
                    if (!isDigit(optarg)) {
                        print_help();
                    }
                    g_right = atoi(optarg);
                    if (g_right < 1 || g_right > 32) {
                        print_help();
                    }
                    num++;
                    break;
                case '?':
                default:
                    print_help();
            } // end swith
        } // end while

        if (num != 0 && num != 2) {
            cerr << "malformed command, need to specify two openings\n";
            exit(1);
        }

        if ((argc - optind != 4) && (argc - optind != 3)) {
            print_help();
        }

        optind++; //scape mazegen

        //fist width
        if (isDigit(argv[optind])) {
            g_width = atoi(argv[optind]);
        } else {
            print_help();
        }

        //then height
        optind++; 
        if (isDigit(argv[optind])) {
            g_height = atoi(argv[optind]);
        } else {
            print_help();
        }

        checkParams();
            
        optind++; 
        if (optind < argc) { 
            g_input_file = argv[optind]; 
        } else {
            g_from_std = true;
        }

        gen_maze();
    } // end else

    return 0;
}


