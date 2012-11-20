#ifndef _SEARCH_HPP_
#define _SEARCH_HPP_

#include "hw4_include.h"
#include "queue.hpp"
#include "problem.hpp"
#include "mazedata.hpp"

class Search
{
public:
    ~Search();


    void insertQueue(State *node);
    State * getFromQueue();
    bool isInQueue(State *node);
    void insertExplored(State *node);
    bool isInExplored(State *node);
    State *findQueueByKey(int key);
    void updateQueue(State *old_node, State *new_node);
    void traceNode(State *node);

    virtual bool doSearch(Problem &problem, std::vector<State*> &path);
    virtual bool doSearch(Problem &problem, std::vector<State*> &path, int &max_level);

    int getExploredSize()
    {
        return _explored.size();
    }

    int getStoredSize()
    {
        return _stored.size();
    }

    void clearMember()
    {
        if (_queue != NULL)
            _queue->clear();
        _explored.clear();
        _stored.clear();
    }

    const char *getName()
    {
        return _name;
    }
public:
    //just for use of debug
    const char *_name;

    Queue<State *> *_queue;

    /*
     * we use the x and y of the node to construct the key of the map
     */
    std::map<int, State *> _explored;

    //stored for easy find in queue
    std::map<int, State *> _stored;
};

class BFSearch : public Search
{
public:
    BFSearch()
    {
        _queue = new FIFOQueue<State *>();
    }

    ~BFSearch()
    {
        while(!_queue->isEmpty())
        {
            delete _queue->pop();
        }
        delete _queue;
    }

    bool doSearch(Problem &problem, std::vector<State *> &path, int &max_level);
};

class DijkSearch : public Search
{
public:
    DijkSearch()
    {
        _queue = new PrioQueue<State *>;
    }

    ~DijkSearch()
    {
        while(!_queue->isEmpty())
        {
            delete _queue->pop();
        }
        delete _queue;
    }

    bool doSearch(Problem &problem, std::vector<State *> &path);

};

#endif
