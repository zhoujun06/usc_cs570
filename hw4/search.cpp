#include "search.hpp"

Search::~Search()
{
    std::map<int, State *>::iterator it;
    for(it = _explored.begin(); it != _explored.end(); it++)
    {
        delete it->second;
        it->second = NULL;
    }

    _explored.clear();
    _stored.clear();
}

void Search::insertQueue(State *node)
{
    _queue->push(node);

    int key = node->getKey();
    _stored.insert(std::pair<int, State *>(key, node));
}

State * Search::getFromQueue()
{
    State * node = _queue->pop();
    int key = node->getKey();

    std::map<int, State *>::iterator it;
    it = _stored.find(key);
    _stored.erase(it);

    return node;
}

State * Search::findQueueByKey(int key)
{
    std::map<int, State *>::iterator it;
    it = _stored.find(key);
    if(it != _stored.end())
    {
        return it->second;
    }

    return NULL;
}

void Search::updateQueue(State *old_node, State *new_node)
{
    //what we have is the pointer to the old_node in the map, 
    //since we cant access a particular node in the priority queue
    //so we cant update the pointer in the queue to pointer to new old
    //thus we do it with a tricky method:
    //we memcpy the contents of new_node to the address of the old one
    //then, although the ptr in the queue is not changed, although it 
    //still points to the old node, but ,actually, it has the contens of 
    //the new node, and then, we just delete the new_node, in case of 
    //memory leak.
    //Since it is in the queue, not explored yet, so no child will point to
    //the old one, thus the memcpy won't bring other by products.
    memcpy(old_node, new_node, sizeof(State));

    delete new_node;
    new_node = NULL;
}

bool Search::isInQueue(State *node)
{
    int key = node->getKey();
    if(_stored.find(key) != _stored.end())
    {
        return true;
    }

    return false;
}

void Search::insertExplored(State *node)
{       
    int key = node->getKey();
    _explored.insert(std::pair<int, State *>(key, node));
}

bool Search::isInExplored(State *node)
{
    int key = node->getKey();
    if(_explored.find(key) != _explored.end())
    {
        return true;
    }

    return false;
}

void Search::traceNode(State *node)
{
#ifdef HW_DEBUG
    cout << "room:" << node->_room->_num;
    cout << " row:" << node->_room->_row;
    cout << " col:" << node->_room->_col;
    cout << endl;
#endif
}

bool Search::doSearch(Problem &pb, std::vector<State *> &path)
{
    return true;
}
bool Search::doSearch(Problem &pb, std::vector<State *> &path, int & max_level)
{
    return true;
}

bool DijkSearch::doSearch(Problem &problem, std::vector<State *> &path)
{
    clearMember();
    State *init = problem.getInitState();
    insertQueue(init);

    while (!_queue->isEmpty())
    {
        State *node = _queue->pop();
        if (problem.isGoal(node))
        {
            node->getPathFromRoot(path);
            return true;
        }
        
        insertExplored(node);
        traceNode(node);
        
        std::vector<RoomDNb> & nbs = node->_room->getNbs();
        std::vector<RoomDNb>::iterator it;

        for (it=nbs.begin(); it<nbs.end(); it++) {
            State *child = NULL;
            problem.getNextState(node, &(*it), &child);

            if(!isInQueue(child) && !isInExplored(child)) {
                insertQueue(child);
            } else {
                if(isInQueue(child))
                {
                    //get old one
                    State *old = findQueueByKey(child->getKey());
                    if(child->getPathCost() < old->getPathCost())
                    {
                        //update;
#ifdef HW2_DEBUG
                        std::cout <<"before update, child cost: " << child->getPathCost();
                        std::cout <<",  old cost: " << old->getPathCost();
#endif
                        updateQueue(old, child);
#ifdef HW2_DEBUG
                        std::cout <<"after,  old cost: " << old->getPathCost();
#endif
                    }
                }
            }
        }
    }// end while

    return false;
}

bool BFSearch::doSearch(Problem &problem, std::vector<State *> &path, int & max_level)
{
    clearMember();
    State *init = problem.getInitState();

    bool done = false;
    max_level = 1;

    insertQueue(init);


    while(!_queue->isEmpty())
    {
        State * node = getFromQueue();
        insertExplored(node);
        traceNode(node);

        if (node->_path_cost > max_level) {
            max_level = node->_path_cost;
        }

        std::vector<RoomDNb> & nbs = node->_room->getNbs();
        std::vector<RoomDNb>::iterator it;

        for (it = nbs.begin(); it<nbs.end(); it++)
        {
            State *child = NULL;
            problem.getNextState(node, &(*it), &child);

            //check if in the queue
            if(!isInQueue(child) && !isInExplored(child))
            {
                if(problem.isGoal(child))
                {
                    if (false == done) {
                        child->getPathFromRoot(path);
                        done = true;
                    }
                    //return true;
                }

                insertQueue(child);
            }
        }
    }

    return done;
}


