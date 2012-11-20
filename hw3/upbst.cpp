#include "bst.hpp"

extern bool g_display_all;

Bst::Bst(istream &input) : _root(NULL), _input(input) {}

Bst::~Bst()
{
    clear(_root);
}

void Bst::doRun()
{
    if (FALSE == build()) {
        return;
    }

    display();
}

int Bst::build()
{
    char line[MAX_LINE+2];
    int num = 0;
    _input.getline(line, sizeof(line));
    while (_input.good()) 
    {
        num++;
        if (strlen(line) > MAX_LINE)
        {
            cerr << "line too long, No: " << num << endl;
            return -1;
        }

        char *str = trim(line);
        Node *node = new Node(str);

        
        insert(node);
        if (g_display_all) {
            display();
        }
        _input.getline(line, sizeof(line));
    }

    return TRUE;
}

char *Bst::trim(char *str)
{
    if (str == NULL) {
        return NULL;
    }

    char *head = str;
    char *end = str + strlen(str)-1;
    while (head != NULL && 
        (*head == ' ' || *head == '\t' || *head == '\r'))
    {
        head++;
    }

    if (head == NULL) 
    {
        return NULL;
    }

    while (*end == ' ' || *end == '\t' || *end == '\r')
    {
        end--;
    }

    *(end+1) = '\0';

    return head;
}

int Bst::insert(Node *node)
{
    if (_root == NULL) 
    {
        _root = node;
        return TRUE;
    }

    Node *p = NULL;
    int res = find(node->_data, p);
    if (res == TRUE) {
        cerr << "conflict key " << node->_data << endl;
        exit(1);
    }
    //otherwise, p is the future parent of the node
    node->_parent = p;
    if (strcmp(p->_data, node->_data) > 0) {
        p->_left = node;
    } else {
        p->_right = node;
    }

    node->_level = p->_level+1;

    return TRUE;
}

int Bst::find(char *str, Node * &p)
{
    return itnFind(_root, str, p);
}

int Bst::itnFind(Node *root, char *str, Node * &p)
{
    if (root == NULL) {
        return FALSE;
    }

    if (strcmp(root->_data, str) == 0) {
        p = root;
        return TRUE;
    } else if (strcmp(root->_data, str) > 0) {
        if (root->_left != NULL) {
            return itnFind(root->_left, str, p);
        } else {
            p = root;
            return FALSE;
        }
    } else if (strcmp(root->_data, str) < 0) {
        if (root->_right != NULL) {
            return itnFind(root->_right, str, p);
        } else {
            p = root;
            return FALSE;
        }
    }

    return FALSE;
}

void Bst::display()
{
    uint64_t bits = 0;
    int level = 0;
    _root->nodeDisplay(bits, level);
}

void Node::printPre(uint64_t bits, int level)
{
    if (level <= 0) {
        return;
    }

    cout << ' ';
    for (int i=1; i<level; i++) {
        if (bits & 1<<i) {
            //at this level, it is the right child
            cout << "     ";
        } else {
            cout << "|    ";
        }
    }
}

void Node::nodeDisplay(uint64_t bits)
{
    printPre(bits, _level);

    if (_level != 0) {
        if (bits & 1<<_level) {
            cout << "\\-- ";
        } else {
            cout << "+-- ";
        } 
    }

    cout << _data;
    cout << endl;

    if (_left == NULL) {
        printPre(bits, _level+1);
        cout << "+-- NULL\n";
    } else {
        _left->nodeDisplay(bits);
    }

    if (_right == NULL) {
        printPre(bits, _level+1);
        cout << "\\-- NULL\n";
    } else {
        bits = bits | 1<<(_level+1);
        _right->nodeDisplay(bits);
    }
}

void Bst::clear(Node *root)
{
    if (root != NULL) {
        clear(root->_left);
        clear(root->_right);
        delete root;
        root = NULL;
    }
}

void Bst::swap(Node *n1, Node *n2)
{
    char *str = n1->_data;
    n1->_data = n2->_data;
    n2->_data = str;
}

Node *Bst::getInOrder(Node *node)
{
    if (NULL == node) {
        return NULL;
    }

    Node *p = node->_right;
    while (p->_left) {
        p = p->_left;
    }

    return p;
}

void Bst::decLevel(Node *node)
{
    if(NULL != node) {
        (node->_level)--;
        decLevel(node->_left);
        decLevel(node->_right);
    }
}

int Bst::del(char *str)
{
    if (NULL == str) {
        cerr << "delete input null str\n";
        exit(1);
    }

    Node *node = NULL;
    if (FALSE == find(str, node)) {
        cerr << "can't find when delete: " << str << endl;
        exit(1);
    }

    if (node->_left != NULL && node->_right != NULL) {
        //find inorder successor
        Node *inorder = getInOrder(node);
        swap(node, inorder);

        if (inorder->_right)
        {
            inorder->_right->_parent = inorder->_parent;
            decLevel(inorder->_right);
        }
        if (inorder->_parent->_left == inorder) {
            inorder->_parent->_left = inorder->_right;
        } else {
            inorder->_parent->_right = inorder->_right;
        }

        return TRUE;
        
    } else if (node->_left != NULL) {
        node->_left->_parent = node->_parent;
        decLevel(node->_left);
        if (node->_parent->_left == node) {
            node->_parent->_left = node->_left;
        } else {
            node->_parent->_right = node->_left;
        }
    } else {
        if (node->_right != NULL) {
            node->_right->_parent = node->_parent;
            decLevel(node->_right);
        }
        if (node->_parent->_left == node) {
            node->_parent->_left = node->_right;
        } else {
            node->_parent->_right = node->_right;
        }
    }

    return TRUE;
}
