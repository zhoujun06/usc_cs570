#ifndef _PROBLEM_HPP_
#define _PROBLEM_HPP_

#include "hw4_include.h"
#include "mazedata.hpp"

class Problem
{
    public:
        Problem(MazeData *maze) : _maze(maze) {}

        /*
         * check whether this state is our goal state, thus the exit room
         */
        bool isGoal(State *st);

        State * getInitState(); 

        /*
         * from each state, we get the next states available
         */
        int getNextState(State *st, RoomDNb * nb, State ** next);



    public:
        /*
         * data of the maze, use adjacent list representation
         * generated from the file input, includes states and edges
         * from this data, we know what actions should each state take,
         * thus where could each state go to the next state(room)
         */
        MazeData * _maze; 
};

#endif
