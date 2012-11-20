#ifndef _BST_HPP_
#define _BST_HPP_

#include "hw3_include.h"


const uint32_t MAX_LINE = 1000;

class Bst;
class Node
{
    public:
    Node(char *str)
    {
        _data = new char[strlen(str)+1];
        strcpy(_data, str);
        _left = NULL;
        _right = NULL;
        _parent = NULL;
        _level = 0;
        _balance = 0;
        _height = 1;
    }
    ~Node()
    {
        delete _data;
    }

    void nodeDisplay(uint64_t bits, int level);
    void printPre(uint64_t bits, int level);

    private:
        friend class Bst;

        Node * _parent;
        Node * _left;
        Node * _right;
        char * _data;
        int _level;
        int _balance;
        int _height;
};

class Bst
{
    public:
    Bst(istream &input);
    ~Bst();

    int build();
    void display();
    void doRun();
    void clear(Node *node);

    char *trim(char *str);
    int del(char *str);
    int insert(Node *node);
    int find(char *str, Node* &p);
    int itnFind(Node *root, char *str, Node* &p);
    void swap(Node *n1, Node *n2);
    Node *getInOrder(Node *node);
    void decLevel(Node *node);

    void checkAvl(Node *node);
    void updateHeight(Node *node);
    void fixHeight(Node *node);
    int  getHeight(Node *node);
    bool isBalance(Node *node);
    Node * rotate(Node *node);
    Node * rotateLL(Node *root);
    Node * rotateRR(Node *root);
    Node * rotateRL(Node *root);
    Node * rotateLR(Node *root);

    private:
        Node * _root;
        istream &_input;
};

#endif
