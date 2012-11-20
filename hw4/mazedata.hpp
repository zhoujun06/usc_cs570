#ifndef _MAZEDATA_HPP_
#define _MAZEDATA_HPP_
#include "hw4_include.h"

class State;
class RoomDNb;
class MazeData;

class RoomD 
{
    public:
        RoomD(int row, int col, int num) 
        {
            _row = row; 
            _col = col;
            _num = num;
        }

        std::vector<RoomDNb> & getNbs() 
        {
            return _nbs;
        }

    public:
        int _row;
        int _col;
        
        /* 
         * use this num to get from the vector in MazeData
         */
        int _num;

        /*
         * use a list to store it's neighbors (RoomDNb)
         * along with the neighbors, we should also store the information
         * of the cost from this room to the neighbor
         */
        //My570List _nbs;
        std::vector<RoomDNb> _nbs;
};

class Edge
{
    public:
        Edge(int cost) {_cost = cost; }


    public:
        /*
         * number of the edge, should be used in the mazegen
         * will be used in magesol? not sure !!!!!!!!!!!!!!!!!!!
         */
        int _num;

        /*
         * cost to pass the edge
         */
        int _cost;
};

/* 
 * RoomDNb is just an room plus an edge
 */
class RoomDNb
{
    public:

        RoomD * _room;
        Edge * _edge;
};

/*
 * RoomDs are static, State are dynamic
 * States refer to a certain room, with dynamic information,
 * like the path_cost, parent information.
 *
 */
class State
{
    public:
        State(State * parent, RoomD *room, int path_cost)
        {
            _parent = parent;
            _room = room;
            _path_cost = path_cost;
        }

        int getKey()
        {
            return _room->_num;
        }

        int getPathCost()
        {
            return _path_cost;
        }

        int getPathFromRoot(std::vector<State *> &path);

    public:
        RoomD * _room;
        /*
         * path cost is the cost from the original state to here
         */
        int _path_cost;

        /*
         * for the path information
         */
        State *_parent;
};

enum MazeType_t
{
    MAZE_TYPE_1 = 1,
    MAZE_TYPE_2 = 2,
};
/* 
 * MazeData is the graph representation of the maze
 * a vector of RoomD, and each room has a list of neighbors.
 * we could use the row and col of a room to find it from the 
 * vector.
 */
class MazeData
{
    public:
        MazeData(istream &input);
        ~MazeData();

        /* 
         * use row and col of a room to get the index in the vector
         */
        int getIndex(int row, int col);

        char *trim(char *str);
        void init();
        void initParam();
        void checkInvalidChar();
        void changeToGraph();
        void printData();
        void printGraph();
        void fillPath(std::vector<State *> &path);
        int checkNb(int i, int j, int ni, int nj);

    public:

        MazeType_t _type;
        int _height;
        int _width;

        istream &  _input;
        /*
         * raw data from input, 2 dimentional array
         */
        std::vector<char *> _data;

        /* 
         * room number of entrance and exit
         * start is the first
         * */
        int _entry[2];

        /*
         * graph representation generated from data above
         */
        std::vector<RoomD> _room;
};

#endif
