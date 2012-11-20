#include "problem.hpp"

State * Problem::getInitState()
{
    int num = _maze->_entry[0];
    State *init = new State(NULL, &(_maze->_room[num]), 0);
    return init;
}

bool Problem::isGoal(State *st)
{
    int num = _maze->_entry[1];
    return st->_room->_num == num;
}

int Problem::getNextState(State *st, RoomDNb *nb, State **next)
{
    int path_cost = st->_path_cost + nb->_edge->_cost;
    int num = nb->_room->_num;

    *next = new State(st, &(_maze->_room[num]), path_cost);
    if (NULL == *next) {
        cerr << "get next state error\n";
        exit(1);
    }

    return 0;
}
