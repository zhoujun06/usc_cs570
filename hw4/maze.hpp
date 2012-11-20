#ifndef _MAZE_HPP_
#define _MAZE_HPP_
#include "hw4_include.h"

class Maze;
class Node{
    public:
        Node();
        ~Node();

    private:
        friend class Maze;

        int _x;
        int _y;
        Node *_parent;
};

class Room {
    public:
        Room(int num) 
        {
            _num = num;
            _parent = NULL;
        }

        int getRoot(Room * & p, int &level);
    private:
        friend class Maze;
        int _num;
        Room * _parent;
};

class Wall{
    public:
        Wall(int num) {_num = num;}

    private:
        friend class Maze;

        int _num;
};

class Maze{

    public:
        Maze(FILE *fp, int height, int width, int top, int bottom, int left, int right);
        ~Maze();

        void initMaze();
        void initData();
        void initGate();
        void initWallList();
        void initRoom();
        int removeWall();
        int getCoFromWallNum(int num, int &row, int &col);
        void printMaze();
        int getRowNum() {return 2 * _height + 1; }
        int getColNum() {return 2 * _width + 1; }
        void build();
        bool getWallNum(uint32_t &num);
        int getAdjRoom(int row, int col, int &room1, int &room2);
        bool isConnected(int room1, int room2);

    private:
        int _height;
        int _width;
        int _top;
        int _bottom;
        int _left;
        int _right;

        //istream &_input;

        FILE *_fp;

        /*
         * _data is (2*_height+1) rows, and (2*_width+1) columns
         */
        char **_data;

        /*
         * for delete the walls
         */
        My570List _list;

        vector<Room> _room;

};




#endif

